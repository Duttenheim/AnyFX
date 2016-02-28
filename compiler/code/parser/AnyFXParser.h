/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : antlr/grammar/AnyFX.g
 *     -                            On : 2016-02-23 19:50:00
 *     -                for the parser : AnyFXParserParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser 
AnyFXParser

has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pAnyFXParser, which is returned from a call to AnyFXParserNew().
 *
 * The methods in pAnyFXParser are  as follows:
 *
 *  - 
 std::string
      pAnyFXParser->string(pAnyFXParser)
 *  - 
 bool
      pAnyFXParser->boolean(pAnyFXParser)
 *  - 
 Effect
      pAnyFXParser->entry(pAnyFXParser)
 *  - 
 Effect
      pAnyFXParser->effect(pAnyFXParser)
 *  - 
 std::string
      pAnyFXParser->qualifier(pAnyFXParser)
 *  - 
 std::string
      pAnyFXParser->qualifierValued(pAnyFXParser)
 *  - 
 DataType
      pAnyFXParser->type(pAnyFXParser)
 *  - 
 QualifierExpression
      pAnyFXParser->qualifierExpression(pAnyFXParser)
 *  - 
 Structure
      pAnyFXParser->structure(pAnyFXParser)
 *  - 
 VarBlock
      pAnyFXParser->varblock(pAnyFXParser)
 *  - 
 VarBuffer
      pAnyFXParser->varbuffer(pAnyFXParser)
 *  - 
 Subroutine
      pAnyFXParser->subroutine(pAnyFXParser)
 *  - 
 ValueList
      pAnyFXParser->valueList(pAnyFXParser)
 *  - 
 ValueList
      pAnyFXParser->valueSingleList(pAnyFXParser)
 *  - 
 Variable
      pAnyFXParser->variable(pAnyFXParser)
 *  - 
 Constant
      pAnyFXParser->constant(pAnyFXParser)
 *  - 
 Parameter::Attribute
      pAnyFXParser->parameterAttribute(pAnyFXParser)
 *  - 
 Parameter
      pAnyFXParser->parameter(pAnyFXParser)
 *  - 
 std::vector<Parameter>
      pAnyFXParser->parameterList(pAnyFXParser)
 *  - 
 FunctionAttribute
      pAnyFXParser->functionAttribute(pAnyFXParser)
 *  - 
 void
      pAnyFXParser->codeBlock(pAnyFXParser)
 *  - 
 Function
      pAnyFXParser->function(pAnyFXParser)
 *  - 
 BlendStateRow
      pAnyFXParser->blendStateRow(pAnyFXParser)
 *  - 
 RenderStateRow
      pAnyFXParser->renderStateRow(pAnyFXParser)
 *  - 
 RenderState
      pAnyFXParser->renderState(pAnyFXParser)
 *  - 
 Sampler
      pAnyFXParser->sampler(pAnyFXParser)
 *  - 
 SamplerTextureList
      pAnyFXParser->samplerTextureList(pAnyFXParser)
 *  - 
 SamplerRow
      pAnyFXParser->samplerRow(pAnyFXParser)
 *  - 
 ProgramRow
      pAnyFXParser->programRow(pAnyFXParser)
 *  - 
 Annotation
      pAnyFXParser->annotation(pAnyFXParser)
 *  - 
 Program
      pAnyFXParser->program(pAnyFXParser)
 *  - 
 Expression*
      pAnyFXParser->expression(pAnyFXParser)
 *  - 
 Expression*
      pAnyFXParser->binaryexp7(pAnyFXParser)
 *  - 
 Expression*
      pAnyFXParser->binaryexp6(pAnyFXParser)
 *  - 
 Expression*
      pAnyFXParser->binaryexp5(pAnyFXParser)
 *  - 
 Expression*
      pAnyFXParser->binaryexp4(pAnyFXParser)
 *  - 
 Expression*
      pAnyFXParser->binaryexp3(pAnyFXParser)
 *  - 
 Expression*
      pAnyFXParser->binaryexp2(pAnyFXParser)
 *  - 
 Expression*
      pAnyFXParser->binaryexp1(pAnyFXParser)
 *  - 
 Expression*
      pAnyFXParser->binaryexpatom(pAnyFXParser)
 *  - 
 Expression*
      pAnyFXParser->parantexpression(pAnyFXParser)
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD license"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_AnyFXParser_H
#define _AnyFXParser_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */
	

// include std container
#include <vector>
#include <typeinfo>
#include <iostream>
#include <string>
#include <stack>

#include "../../code/qualifierexpression.h"
#include "../../code/compileable.h"
#include "../../code/effect.h"
#include "../../code/header.h"
#include "../../code/datatype.h"
#include "../../code/parameter.h"
#include "../../code/annotation.h"
#include "../../code/programrow.h"
#include "../../code/program.h"
#include "../../code/samplertexturelist.h"
#include "../../code/samplerrow.h"
#include "../../code/sampler.h"
#include "../../code/structure.h"
#include "../../code/constant.h"
#include "../../code/blendstaterow.h"
#include "../../code/renderstaterow.h"
#include "../../code/renderstate.h"
#include "../../code/function.h"
#include "../../code/valuelist.h"
#include "../../code/functionattribute.h"
#include "../../code/variable.h"
#include "../../code/varblock.h"
#include "../../code/varbuffer.h"
#include "../../code/subroutine.h"
#include "../../code/preprocessor.h"
#include "../../code/expressions/expression.h"
#include "../../code/expressions/binaryexpression.h"
#include "../../code/expressions/unaryexpression.h"
#include "../../code/expressions/intexpression.h"
#include "../../code/expressions/boolexpression.h"
#include "../../code/expressions/floatexpression.h"
using namespace AnyFX;



#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct AnyFXParser_Ctx_struct AnyFXParser, * pAnyFXParser;



#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/* ========================
 * BACKTRACKING IS ENABLED
 * ========================
 */


/** Context tracking structure for 
AnyFXParser

 */
struct AnyFXParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;

     std::string
     (*string)	(struct AnyFXParser_Ctx_struct * ctx);

     bool
     (*boolean)	(struct AnyFXParser_Ctx_struct * ctx);

     Effect
     (*entry)	(struct AnyFXParser_Ctx_struct * ctx);

     Effect
     (*effect)	(struct AnyFXParser_Ctx_struct * ctx);

     std::string
     (*qualifier)	(struct AnyFXParser_Ctx_struct * ctx);

     std::string
     (*qualifierValued)	(struct AnyFXParser_Ctx_struct * ctx);

     DataType
     (*type)	(struct AnyFXParser_Ctx_struct * ctx);

     QualifierExpression
     (*qualifierExpression)	(struct AnyFXParser_Ctx_struct * ctx);

     Structure
     (*structure)	(struct AnyFXParser_Ctx_struct * ctx);

     VarBlock
     (*varblock)	(struct AnyFXParser_Ctx_struct * ctx);

     VarBuffer
     (*varbuffer)	(struct AnyFXParser_Ctx_struct * ctx);

     Subroutine
     (*subroutine)	(struct AnyFXParser_Ctx_struct * ctx);

     ValueList
     (*valueList)	(struct AnyFXParser_Ctx_struct * ctx);

     ValueList
     (*valueSingleList)	(struct AnyFXParser_Ctx_struct * ctx);

     Variable
     (*variable)	(struct AnyFXParser_Ctx_struct * ctx);

     Constant
     (*constant)	(struct AnyFXParser_Ctx_struct * ctx);

     Parameter::Attribute
     (*parameterAttribute)	(struct AnyFXParser_Ctx_struct * ctx);

     Parameter
     (*parameter)	(struct AnyFXParser_Ctx_struct * ctx);

     std::vector<Parameter>
     (*parameterList)	(struct AnyFXParser_Ctx_struct * ctx);

     FunctionAttribute
     (*functionAttribute)	(struct AnyFXParser_Ctx_struct * ctx);

     void
     (*codeBlock)	(struct AnyFXParser_Ctx_struct * ctx);

     Function
     (*function)	(struct AnyFXParser_Ctx_struct * ctx);

     BlendStateRow
     (*blendStateRow)	(struct AnyFXParser_Ctx_struct * ctx);

     RenderStateRow
     (*renderStateRow)	(struct AnyFXParser_Ctx_struct * ctx);

     RenderState
     (*renderState)	(struct AnyFXParser_Ctx_struct * ctx);

     Sampler
     (*sampler)	(struct AnyFXParser_Ctx_struct * ctx);

     SamplerTextureList
     (*samplerTextureList)	(struct AnyFXParser_Ctx_struct * ctx);

     SamplerRow
     (*samplerRow)	(struct AnyFXParser_Ctx_struct * ctx);

     ProgramRow
     (*programRow)	(struct AnyFXParser_Ctx_struct * ctx);

     Annotation
     (*annotation)	(struct AnyFXParser_Ctx_struct * ctx);

     Program
     (*program)	(struct AnyFXParser_Ctx_struct * ctx);

     Expression*
     (*expression)	(struct AnyFXParser_Ctx_struct * ctx);

     Expression*
     (*binaryexp7)	(struct AnyFXParser_Ctx_struct * ctx);

     Expression*
     (*binaryexp6)	(struct AnyFXParser_Ctx_struct * ctx);

     Expression*
     (*binaryexp5)	(struct AnyFXParser_Ctx_struct * ctx);

     Expression*
     (*binaryexp4)	(struct AnyFXParser_Ctx_struct * ctx);

     Expression*
     (*binaryexp3)	(struct AnyFXParser_Ctx_struct * ctx);

     Expression*
     (*binaryexp2)	(struct AnyFXParser_Ctx_struct * ctx);

     Expression*
     (*binaryexp1)	(struct AnyFXParser_Ctx_struct * ctx);

     Expression*
     (*binaryexpatom)	(struct AnyFXParser_Ctx_struct * ctx);

     Expression*
     (*parantexpression)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred5_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred6_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred8_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred11_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred12_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred14_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred79_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred80_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred81_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred83_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred84_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred85_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred86_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred87_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred89_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred90_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred92_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred93_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred95_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred111_AnyFX)	(struct AnyFXParser_Ctx_struct * ctx);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct AnyFXParser_Ctx_struct * ctx);
    void	    (*free)   (struct AnyFXParser_Ctx_struct * ctx);
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pAnyFXParser AnyFXParserNew         (
pANTLR3_COMMON_TOKEN_STREAM
 instream);
ANTLR3_API pAnyFXParser AnyFXParserNewSSD      (
pANTLR3_COMMON_TOKEN_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
parser
 will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif
#define EOF      -1
#define T__49      49
#define T__50      50
#define T__51      51
#define T__52      52
#define T__53      53
#define T__54      54
#define T__55      55
#define T__56      56
#define T__57      57
#define T__58      58
#define T__59      59
#define T__60      60
#define T__61      61
#define T__62      62
#define T__63      63
#define T__64      64
#define T__65      65
#define T__66      66
#define T__67      67
#define T__68      68
#define T__69      69
#define T__70      70
#define T__71      71
#define T__72      72
#define T__73      73
#define T__74      74
#define T__75      75
#define T__76      76
#define T__77      77
#define T__78      78
#define T__79      79
#define T__80      80
#define T__81      81
#define T__82      82
#define T__83      83
#define T__84      84
#define T__85      85
#define T__86      86
#define T__87      87
#define T__88      88
#define T__89      89
#define T__90      90
#define T__91      91
#define T__92      92
#define T__93      93
#define T__94      94
#define T__95      95
#define T__96      96
#define T__97      97
#define T__98      98
#define T__99      99
#define T__100      100
#define T__101      101
#define T__102      102
#define T__103      103
#define T__104      104
#define T__105      105
#define T__106      106
#define T__107      107
#define T__108      108
#define T__109      109
#define T__110      110
#define T__111      111
#define T__112      112
#define T__113      113
#define T__114      114
#define T__115      115
#define ADD      4
#define ALPHABET      5
#define AND      6
#define CO      7
#define COL      8
#define COMMENT      9
#define DIV      10
#define DOT      11
#define DOUBLELITERAL      12
#define EQ      13
#define EXPONENT      14
#define FLOATLITERAL      15
#define FORWARDSLASH      16
#define GREATER      17
#define GREATEREQ      18
#define HEX      19
#define IDENTIFIER      20
#define INTEGER      21
#define INTEGERLITERAL      22
#define LB      23
#define LESS      24
#define LESSEQ      25
#define LL      26
#define LOGICAND      27
#define LOGICEQ      28
#define LOGICOR      29
#define LP      30
#define ML_COMMENT      31
#define MOD      32
#define MUL      33
#define NOT      34
#define NOTEQ      35
#define NU      36
#define OR      37
#define PATH      38
#define PREPROCESSOR      39
#define Q      40
#define QO      41
#define QU      42
#define RB      43
#define RP      44
#define RR      45
#define SC      46
#define SUB      47
#define WS      48
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for AnyFXParser
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
