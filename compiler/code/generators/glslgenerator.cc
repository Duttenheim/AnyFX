//------------------------------------------------------------------------------
//	AnyFX generator functions for GLSL based code generation
//	(C) 2016 Gustav Sterbrant
//------------------------------------------------------------------------------
#include <string>
#include "shader.h"
#include "generator.h"
#include "programrow.h"

namespace AnyFX
{
//------------------------------------------------------------------------------
/**
	Generate GLSL code from shader
*/
std::string
Shader::GenerateGLSL(AnyFX::Generator* generator, int major, int minor)
{
	switch (major)
	{
		case 4: return GenerateGLSL4(generator);
		case 3: return GenerateGLSL3(generator);
	}
	return "";
}

//------------------------------------------------------------------------------
/**
	Generate GLSL as GLSL 4
*/
std::string
Shader::GenerateGLSL4(AnyFX::Generator* generator)
{
	std::string code;

	// get header type
	const Header& header = generator->GetHeader();

	// write function attributes
	if (this->shaderType == ProgramRow::VertexShader)
	{
		// type checker should throw warning/error if we have an attribute
	}
	else if (this->shaderType == ProgramRow::PixelShader)
	{
		if (this->func.HasBoolFlag(FunctionAttribute::EarlyDepth))
		{
			code.append("layout(early_fragment_tests) in;\n");
		}
	}
	else if (this->shaderType == ProgramRow::HullShader)
	{
		bool hasOutputSize = this->func.HasIntFlag(FunctionAttribute::OutputVertices);
		if (!hasOutputSize)
		{
			std::string err = Format("Hull shader '%s' requires [outputvertices] to be defined, %s\n", this->name.c_str(), this->ErrorSuffix().c_str());
			generator->Error(err);
		}

		int outputSize = this->func.GetIntFlag(FunctionAttribute::OutputVertices);
		code.append("layout(vertices = " + Format("%d", outputSize) + ") out;\n");
	}
	else if (this->shaderType == ProgramRow::DomainShader)
	{
		bool hasVertexCount = this->func.HasIntFlag(FunctionAttribute::InputVertices);
		bool hasInputTopology = this->func.HasIntFlag(FunctionAttribute::Topology);
		if (!hasVertexCount)
		{
			std::string err = Format("Domain shader '%s' requires [inputvertices] to be defined, %s\n", this->name.c_str(), this->ErrorSuffix().c_str());
			generator->Error(err);
		}
		// input topology and spacing is not optional
		if (!hasInputTopology)
		{
			std::string err = Format("Domain shader '%s' requires [topology] to be defined, %s\n", this->name.c_str(), this->ErrorSuffix().c_str());
			generator->Error(err);
		}

		int vertexCount = this->func.GetIntFlag(FunctionAttribute::InputVertices);
		int inputTopology = this->func.GetIntFlag(FunctionAttribute::Topology);
		bool hasSpacing = this->func.HasIntFlag(FunctionAttribute::PartitionMethod);
		bool hasWindingOrder = this->func.HasIntFlag(FunctionAttribute::WindingOrder);

		code.append("layout(");

		// write topology
		switch (inputTopology)
		{
		case 0:			// isolines
			code.append("triangles");
			break;
		case 1:			// triangles
			code.append("quads");
			break;
		case 2:			// quads
			code.append("isolines");
			break;
		case 3:			// force points
			code.append("point_mode");
			break;
		}

		if (hasSpacing)
		{
			// add comma for spacing method
			code.append(", ");
			int spacing = this->func.GetIntFlag(FunctionAttribute::PartitionMethod);

			switch (spacing)
			{
			case 0:
				code.append("equal_spacing");
				break;
			case 1:
				code.append("fractional_even_spacing");
				break;
			case 2:
				code.append("fractional_odd_spacing");
				break;
			case 3:
			{
				std::string message = Format("Tessellation Evaluation Shader '%s' does not define partitioning method 'pow' for GLSL4, %d:%d\n", this->name.c_str(), this->line, this->row);
				generator->Error(message);
				break;
			}
			}
		}

		if (hasWindingOrder)
		{
			// add comma for winding order
			code.append(", ");
			int winding = this->func.GetIntFlag(FunctionAttribute::WindingOrder);

			switch (winding)
			{
			case 0:
				code.append("cw");
				break;
			case 1:
				code.append("ccw");
				break;
			}
		}

		// write end of evaluation shader attributes
		code.append(") in;\n");
	}
	else if (this->shaderType == ProgramRow::GeometryShader)
	{
		bool hasInput = this->func.HasIntFlag(FunctionAttribute::InputPrimitive);
		bool hasOutput = this->func.HasIntFlag(FunctionAttribute::OutputPrimitive);
		bool hasMaxVerts = this->func.HasIntFlag(FunctionAttribute::MaxVertexCount);
		bool hasInstances = this->func.HasIntFlag(FunctionAttribute::Instances);

		if (!hasInput)
		{
			return "";
		}
		if (!hasOutput)
		{
			return "";
		}
		if (!hasMaxVerts)
		{
			return "";
		}

		// write input primitive type
		{
			int type = this->func.GetIntFlag(FunctionAttribute::InputPrimitive);
			std::string inLayout;

			switch (type)
			{
			case 0:			// points
				inLayout.append("points");
				break;
			case 1:			// lines
				inLayout.append("lines");
				break;
			case 2:			// lines_adjacency
				inLayout.append("lines_adjacency");
				break;
			case 3:			// triangles
				inLayout.append("triangles");
				break;
			case 4:			// triangles_adjacency
				inLayout.append("triangles_adjacency");
				break;
			}

			// append instances if required
			if (hasInstances)
			{
				inLayout.append(AnyFX::Format(", invocations = %d", this->func.GetIntFlag(FunctionAttribute::Instances)));
			}
			code.append(AnyFX::Format("layout(%s) in;\n", inLayout.c_str()));
		}

		// write output primitive type
		{
			int type = this->func.GetIntFlag(FunctionAttribute::OutputPrimitive);
			int maxVerts = this->func.GetIntFlag(FunctionAttribute::MaxVertexCount);
			switch (type)
			{
			case 0:			// points
				code.append("layout(points, max_vertices = " + Format("%d", maxVerts) + ") out;\n");
				break;
			case 1:			// line_strip
				code.append("layout(line_strip, max_vertices = " + Format("%d", maxVerts) + ") out;\n");
				break;
			case 2:			// triangle_strip
				code.append("layout(triangle_strip, max_vertices = " + Format("%d", maxVerts) + ") out;\n");
				break;
			}
		}
	}
	else if (this->shaderType == ProgramRow::ComputeShader)
	{
		bool hasLocalX = this->func.HasIntFlag(FunctionAttribute::LocalSizeX);
		bool hasLocalY = this->func.HasIntFlag(FunctionAttribute::LocalSizeY);
		bool hasLocalZ = this->func.HasIntFlag(FunctionAttribute::LocalSizeZ);

		if (hasLocalX || hasLocalY || hasLocalZ)
		{
			code.append("layout(local_size_x = ");
			if (hasLocalX)
			{
				std::string number = AnyFX::Format("%d", this->func.GetIntFlag(FunctionAttribute::LocalSizeX));
				code.append(number);
			}
			else
			{
				code.append("1");
			}

			code.append(", ");
			code.append("local_size_y = ");

			if (hasLocalY)
			{
				std::string number = AnyFX::Format("%d", this->func.GetIntFlag(FunctionAttribute::LocalSizeY));
				code.append(number);
			}
			else
			{
				code.append("1");
			}

			code.append(", ");
			code.append("local_size_z = ");

			if (hasLocalZ)
			{
				std::string number = AnyFX::Format("%d", this->func.GetIntFlag(FunctionAttribute::LocalSizeZ));
				code.append(number);
			}
			else
			{
				code.append("1");
			}
			code.append(") in;\n");
		}
	}

	unsigned input, output;
	input = output = 0;

	unsigned i;
	const unsigned numParams = this->func.GetNumParameters();
	for (i = 0; i < numParams; i++)
	{
		const Parameter* param = this->func.GetParameter(i);

		// format parameter and add it to the code
		code.append(param->Format(header, input, output));
	}

	// add function header
	std::string returnType = DataType::ToProfileType(this->func.GetReturnType(), header.GetType());
	std::string line = Format("#line %d \"%s\"\n", this->func.GetFunctionLine(), this->func.GetFile().c_str());
	code.append(line);
	code.append(returnType);
	code.append("\nmain()\n{\n");
	line = Format("#line %d \"%s\"\n", this->func.GetCodeLine(), this->func.GetFile().c_str());
	code.append(line);
	code.append(func.GetCode());
	code.append("\n}\n");

	// if we don't have subroutines, find and replace names of subroutines with generated functions
	if (header.GetFlags() & Header::NoSubroutines)
	{
		std::map<std::string, std::string>::const_iterator it;
		for (it = this->subroutineMappings.begin(); it != this->subroutineMappings.end(); it++)
		{
			const std::string& find = (*it).first;
			const std::string& replace = (*it).second;
			while (code.find(find) != std::string::npos)
			{
				size_t start = code.find(find);
				code.replace(start, find.length(), replace);
			}
		}
	}

	// set formatted code as the code we just generated
	this->formattedCode = code;
	return code;
}

//------------------------------------------------------------------------------
/**
	Generate GLSL as GLSL 3
*/
std::string
Shader::GenerateGLSL3(AnyFX::Generator* generator)
{
	return "";
}

}