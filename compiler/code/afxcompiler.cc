//------------------------------------------------------------------------------
/**
    AnyFX compiler functions
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "cmdlineargs.h"
#include "afxcompiler.h"
#include "antlr3.h"
#include "parser/AnyFXLexer.h"
#include "parser/AnyFXParser.h"
#include "lexererrorhandler.h"
#include "parsererrorhandler.h"
#include "typechecker.h"
#include "generator.h"
#include "header.h"
#include <fstream>
#include <algorithm>

#if __linux__
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#undef Success
#elif APPLE
#include <libproc.h>
#include <OpenGL/CGLCurrent.h>
#include <OpenGL/CGLTypes.h>
#include <OpenGL/OpenGL.h>
#endif

extern bool lexerError;
extern std::string lexerErrorBuffer;
extern bool parserError;
extern std::string parserErrorBuffer;

#include "mcpp_lib.h"
#include "glslang/Public/ShaderLang.h"

#if WIN32
//------------------------------------------------------------------------------
/**
*/
LRESULT CALLBACK
AnyFXWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

    HDC hDc;
    HGLRC hRc;      
    HACCEL hAccel;
    HINSTANCE hInst;
    HWND hWnd;
#elif __linux__
    Display* dsp;
    Window root;
    GLint attrs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    XVisualInfo* vi;
    Colormap cmap;
    XSetWindowAttributes swa;
    Window win;
    GLXContext glc;
    XWindowAttributes gwa;
    XEvent xev;
#elif APPLE
    CGLContextObj ctx; 
    CGLPixelFormatObj pix; 
    GLint npix; 
    CGLPixelFormatAttribute attribs[] = { 
        (CGLPixelFormatAttribute) 0
    }; 
#endif

//------------------------------------------------------------------------------
/**
*/
bool
AnyFXPreprocess(const std::string& file, const std::vector<std::string>& defines, const std::string& vendor, std::string& output)
{
    std::string fileName = file.substr(file.rfind("/")+1, file.length()-1);
    std::string curDir(file);
    unsigned lastSlash = curDir.rfind("/");
    curDir = curDir.substr(0, lastSlash);

	std::string vend = "-DVENDOR=" + vendor;

    const unsigned numargs = 5 + defines.size();
    std::string* args = new std::string[numargs];
	args[0] = std::string("-C");
	args[1] = std::string("-W 0");
	args[2] = std::string("-a");
	args[3] = vend.c_str();
    //args[2] = "-I" + curDir;
    unsigned i;
    for (i = 0; i < defines.size(); i++)
    {
        args[4 + i] = defines[i].c_str();
    }
    args[numargs-1] = file;

    const char** arguments = new const char*[numargs];
    
    for (i = 0; i < numargs; i++)
    {
        arguments[i] = args[i].c_str();
    }

    // run preprocessing
    mcpp_use_mem_buffers(1);
    int result = mcpp_lib_main(numargs, (char**)arguments);
    if (result != 0)
    {
        delete[] args;
		delete[] arguments;
        return false;
    }
    else
    {
        char* preprocessed = mcpp_get_mem_buffer(OUT);
        output.append(preprocessed);
		delete[] args;
		delete[] arguments;
        return true;
    }
}

//------------------------------------------------------------------------------
/**
    Compiles AnyFX effect.

    @param file			Input file to compile
    @param output		Output destination file
    @param target		Target language
	@param vendor		GPU vendor name
    @param defines		List of preprocessor definitions
    @param errorBuffer	Buffer containing errors, created in function but must be deleted manually
*/
bool
AnyFXCompile(const std::string& file, const std::string& output, const std::string& target, const std::string& vendor, const std::vector<std::string>& defines, const std::vector<std::string>& flags, AnyFXErrorBlob** errorBuffer)
{
    std::string preprocessed;
    (*errorBuffer) = NULL;

    // if preprocessor is successful, continue parsing the actual code
	if (AnyFXPreprocess(file, defines, vendor, preprocessed))
    {
        // when we preprocess, we save it 
        pANTLR3_INPUT_STREAM input;
        pAnyFXLexer lex;
        pAnyFXParser parser;
        pANTLR3_COMMON_TOKEN_STREAM tokens;

        input = antlr3StringStreamNew((pANTLR3_UINT8)preprocessed.c_str(), ANTLR3_ENC_UTF8, preprocessed.size(), (pANTLR3_UINT8)file.c_str());
        lex = AnyFXLexerNew(input);
        tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
        parser = AnyFXParserNew(tokens);

        // create new effect
        EffectEntryPoint effect = parser->entry(parser);

        // stop the process if lexing or parsing fails
        if (!lexerError && !parserError)
        {
            // create header
            Header header;
            header.SetProfile(target);
			header.SetFlags(flags);

            // set effect header and setup effect
            effect.SetHeader(header);
            effect.Setup();

            // create type checker
            TypeChecker typeChecker;

            // type check effect
            typeChecker.SetHeader(header);
            effect.TypeCheck(typeChecker);

            // compile effect
            int typeCheckerStatus = typeChecker.GetStatus();
            if (typeCheckerStatus == TypeChecker::Success || typeCheckerStatus == TypeChecker::Warnings)
            {
                // create code generator
                Generator generator;

                // generate code for effect
                generator.SetHeader(header);
                effect.Generate(generator);

                // set warnings as 'error' buffer
                if (typeCheckerStatus == TypeChecker::Warnings)
                {
                    unsigned warnings = typeChecker.GetWarningCount();
                    std::string errorMessage;
                    errorMessage = typeChecker.GetErrorBuffer();
                    errorMessage = errorMessage + Format("Type checking returned with %d warnings\n", warnings);

                    *errorBuffer = new AnyFXErrorBlob;
                    (*errorBuffer)->buffer = new char[errorMessage.size()];
                    (*errorBuffer)->size = errorMessage.size();
                    errorMessage.copy((*errorBuffer)->buffer, (*errorBuffer)->size);
                    (*errorBuffer)->buffer[(*errorBuffer)->size-1] = '\0';
                }

                if (generator.GetStatus() == Generator::Success)
                {
                    // create binary writer
                    BinWriter writer;
                    writer.SetPath(output);
                    if (writer.Open())
                    {
                        // compile and write to binary writer
                        effect.Compile(writer);

                        // close writer and finish file
                        writer.Close();

						// destroy compiler state and return
						parser->free(parser);
						tokens->free(tokens);
						lex->free(lex);
						input->free(input);				
						mcpp_use_mem_buffers(1);	// clear mcpp
                        return true;
                    }
                    else
                    {
                        std::string errorMessage = Format("File '%s' could not be opened for writing\n", output.c_str());
                        *errorBuffer = new AnyFXErrorBlob;
                        (*errorBuffer)->buffer = new char[errorMessage.size()];
                        (*errorBuffer)->size = errorMessage.size();
                        errorMessage.copy((*errorBuffer)->buffer, (*errorBuffer)->size);
                        (*errorBuffer)->buffer[(*errorBuffer)->size-1] = '\0';

						// destroy compiler state and return
						parser->free(parser);
						tokens->free(tokens);
						lex->free(lex);
						input->free(input);
						mcpp_use_mem_buffers(1);	// clear mcpp
                        return false;
                    }
                }
                else
                {
                    unsigned errors = generator.GetErrorCount();
                    unsigned warnings = generator.GetWarningCount();
                    std::string errorMessage;
                    errorMessage = generator.GetErrorBuffer();
                    errorMessage = errorMessage + Format("Code generation failed with %d errors and %d warnings\n", errors, warnings);

                    *errorBuffer = new AnyFXErrorBlob;
                    (*errorBuffer)->buffer = new char[errorMessage.size()];
                    (*errorBuffer)->size = errorMessage.size();
                    errorMessage.copy((*errorBuffer)->buffer, (*errorBuffer)->size);
                    (*errorBuffer)->buffer[(*errorBuffer)->size-1] = '\0';

					// destroy compiler state and return
					parser->free(parser);
					tokens->free(tokens);
					lex->free(lex);
					input->free(input);
					mcpp_use_mem_buffers(1);	// clear mcpp
                    return false;
                }
            }
            else
            {
                unsigned errors = typeChecker.GetErrorCount();
                unsigned warnings = typeChecker.GetWarningCount();
                std::string errorMessage;
                errorMessage = typeChecker.GetErrorBuffer();
                errorMessage = errorMessage + Format("Type checking failed with %d errors and %d warnings\n", errors, warnings);

                *errorBuffer = new AnyFXErrorBlob;
                (*errorBuffer)->buffer = new char[errorMessage.size()];
                (*errorBuffer)->size = errorMessage.size();
                errorMessage.copy((*errorBuffer)->buffer, (*errorBuffer)->size);
                (*errorBuffer)->buffer[(*errorBuffer)->size-1] = '\0';

				// destroy compiler state and return
				parser->free(parser);
				tokens->free(tokens);
				lex->free(lex);
				input->free(input);
				mcpp_use_mem_buffers(1);	// clear mcpp
                return false;
            }
        }
        else
        {
            std::string errorMessage;
            errorMessage.append(lexerErrorBuffer);
            errorMessage.append(parserErrorBuffer);
            lexerError = false;
            parserError = false;
            lexerErrorBuffer.clear();
            parserErrorBuffer.clear();

            *errorBuffer = new AnyFXErrorBlob;
            (*errorBuffer)->buffer = new char[errorMessage.size()];
            (*errorBuffer)->size = errorMessage.size();
            errorMessage.copy((*errorBuffer)->buffer, (*errorBuffer)->size);
            (*errorBuffer)->buffer[(*errorBuffer)->size-1] = '\0';

			// destroy compiler state and return
			parser->free(parser);
			tokens->free(tokens);
			lex->free(lex);
			input->free(input);
			mcpp_use_mem_buffers(1);	// clear mcpp
            return false;
        }
    }
    else
    {
        char* err = mcpp_get_mem_buffer(ERR);
        if (err)
        {
            size_t size = strlen(err);
            *errorBuffer = new AnyFXErrorBlob;
            (*errorBuffer)->buffer = new char[size];
            (*errorBuffer)->size = size;
            memcpy((void*)(*errorBuffer)->buffer, (void*)err, size);
            (*errorBuffer)->buffer[size-1] = '\0';
			mcpp_use_mem_buffers(1);	// clear mcpp
        }

        return false;
    }	
}

//------------------------------------------------------------------------------
/**
    Run before compilation
*/
void
AnyFXBeginCompile()
{
	glslang::InitializeProcess();
}

//------------------------------------------------------------------------------
/**
    Run after compilation
*/
void
AnyFXEndCompile()
{
	glslang::FinalizeProcess();
}
