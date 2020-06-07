//
// Created by Vladimir Shubarin on 06.06.2020.
//

#ifndef SPO_COMPILER_SRC_TRANSLATOR_HPP_
#define SPO_COMPILER_SRC_TRANSLATOR_HPP_

#include "Parser.h"
class Translator {
  Node *tree{};
  VariableTable *localVariables;
  VariableTable *globalVariables;
  FunctionTable *functions;
  std::string program;
  std::ofstream *outputFile;
 public:
  explicit Translator(SemanticAnalyzer *_semanticAnalyzer) {
	if (_semanticAnalyzer == nullptr) {
	  throw std::runtime_error("ERROR: Translator::Translator(SemanticAnalyzer*) SemanticAnalyzer is NULL");
	}
	localVariables = _semanticAnalyzer->getVariables();
	globalVariables = _semanticAnalyzer->getGlobalVariables();
	functions = _semanticAnalyzer->getFunctions();
	tree = _semanticAnalyzer->getTree();
	outputFile = new std::ofstream;
	translate();
  }
  void translate() {
	outputFile->open("output.asm");
	*outputFile << "global _start\n";
	writeFunctions();
	writeVariables();
	writeMain();
	outputFile->close();
  }
  void writeFunctions() {
	for (auto &func:functions->getFunctions()) {
	  *outputFile << func->getName() << ":\n";
	}
  }
  void writeVariables() {
	*outputFile << "section .data\n";
	for (auto &var:localVariables->getVariables()) {
	  *outputFile << "\t" << var->getName() << "\t";
	  switch (var->getType()) {
		case Variable::INTEGER: *outputFile << "RESD\t 1";
		  break;
		case Variable::DOUBLE:
		case Variable::REAL: *outputFile << "RESQ\t 1";
		  break;
		case Variable::SHORTINT:
		case Variable::SMALLINT:
		case Variable::BYTE: *outputFile << "RESB\t 1";
		  break;
		case Variable::LONGINT: *outputFile << "RESQ\t 1";
		  break;
		case Variable::WORD: *outputFile << "RESW\t 1";
		  break;
		case Variable::CHAR: *outputFile << "RESB\t 1";
		  break;
		case Variable::WIDECHAR: *outputFile << "RESD\t 1";
		  break;
		case Variable::STRING:break;
		case Variable::UNKNOWN:throw std::runtime_error("Got var of unknown type");
	  }
	  *outputFile << "; Var of type: " << Variable::varTypeToString(var->getType()) << "\n";
	}
	for (auto &var:globalVariables->getVariables()) {
	  *outputFile << "global " << var->getName() << "\n";
	  *outputFile << var->getName() << ":\n\t";
	  switch (var->getType()) {
		case Variable::INTEGER: *outputFile << "RESD\t 1";
		  break;
		case Variable::DOUBLE:
		case Variable::REAL: *outputFile << "RESQ\t 1";
		  break;
		case Variable::SHORTINT:
		case Variable::SMALLINT:
		case Variable::BYTE: *outputFile << "RESB\t 1";
		  break;
		case Variable::LONGINT: *outputFile << "RESQ\t 1";
		  break;
		case Variable::WORD: *outputFile << "RESW\t 1";
		  break;
		case Variable::CHAR: *outputFile << "RESB\t 1";
		  break;
		case Variable::WIDECHAR: *outputFile << "RESD\t 1";
		  break;
		case Variable::STRING:break;
		case Variable::UNKNOWN:throw std::runtime_error("Got var of unknown type");
	  }
	  *outputFile << "; Global Var of type: " << Variable::varTypeToString(var->getType()) << "\n\n";
	}
  }
  void writeMain() {
	*outputFile << "_start:\n" << "mov eax,1 ;1 is the number of the exit system call\n"
								  "mov ebx,0 ;Return code- 0 indicates success, if this is an error condition you might want to return something else\n"
								  "int 0x80 ;Transfer control to the kernel\n";
  }
};

#endif //SPO_COMPILER_SRC_TRANSLATOR_HPP_
