/**
 * @file MathExpressionEngineExptrk.h
 * @brief Header file for class MathExpressionEngineExptrk
 * @date Jan 15, 2021 TODO Verify the value and format of the date
 * @author ferrog TODO Verify the name and format of the author
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing,
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class MathExpressionEngineExptrk
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATHEXPRESSIONENGINEEXPTRK_H_
#define MATHEXPRESSIONENGINEEXPTRK_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "exprtk.hpp"
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include <common/BasicTypes.h>
#include <common/AnyValue.h>
#include <common/SharedReference.h>
#include <common/AnyType.h>
#include <common/log-api.h>

#include "Workspace.h"
#include "MathExpressionEngineI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace sup {

namespace sequencer {

class BasicCastI {
public:
    BasicCastI();
    virtual ~BasicCastI();
    virtual void Do(void *in,
                    void *out,
                    ccs::types::uint32 idx)=0;
};

template<typename Tin, typename Tout>
class BasicCast: public BasicCastI {
public:
    BasicCast();
    virtual ~BasicCast();
    virtual void Do(void *in,
                    void *out,
                    ccs::types::uint32 idx);
};

template<typename T>
class MathExpressionEngineExptrk: public MathExpressionEngineI {

private:
    std::vector<std::string> varNames;
    std::vector<std::vector<T> > varMem;
    std::vector<BasicCastI*> varInCast;
    std::vector<BasicCastI*> varOutCast;
    ::ccs::types::uint64 numOfSignalVariables;

    exprtk::symbol_table<T> symbolTable; //<! Stores variables.
    exprtk::parser<T> expressionParser; //<! Parses the expressions.
    exprtk::expression<T> expression; //<! Stores one expression for each output signal declared in the configuration file.

    BasicCast<T, ccs::types::boolean> toBoolCast;
    BasicCast<T, ccs::types::char8> toChar8Cast;
    BasicCast<T, ccs::types::char8> toStringCast;
    BasicCast<T, ccs::types::int8> toInt8Cast;
    BasicCast<T, ccs::types::uint8> toUint8Cast;
    BasicCast<T, ccs::types::int16> toInt16Cast;
    BasicCast<T, ccs::types::uint16> toUint16Cast;
    BasicCast<T, ccs::types::int32> toInt32Cast;
    BasicCast<T, ccs::types::uint32> toUint32Cast;
    BasicCast<T, ccs::types::int64> toInt64Cast;
    BasicCast<T, ccs::types::uint64> toUint64Cast;
    BasicCast<T, ccs::types::float32> toFloat32Cast;
    BasicCast<T, ccs::types::float64> toFloat64Cast;

    BasicCast<ccs::types::boolean, T> fromBoolCast;
    BasicCast<ccs::types::char8, T> fromChar8Cast;
    BasicCast<ccs::types::char8, T> fromStringCast;
    BasicCast<ccs::types::int8, T> fromInt8Cast;
    BasicCast<ccs::types::uint8, T> fromUint8Cast;
    BasicCast<ccs::types::int16, T> fromInt16Cast;
    BasicCast<ccs::types::uint16, T> fromUint16Cast;
    BasicCast<ccs::types::int32, T> fromInt32Cast;
    BasicCast<ccs::types::uint32, T> fromUint32Cast;
    BasicCast<ccs::types::int64, T> fromInt64Cast;
    BasicCast<ccs::types::uint64, T> fromUint64Cast;
    BasicCast<ccs::types::float32, T> fromFloat32Cast;
    BasicCast<ccs::types::float64, T> fromFloat64Cast;

    BasicCastI *inputCasts[14];
    BasicCastI *outputCasts[14];

    Workspace *myWs;

    std::string::size_type CheckForArray(std::string varName,
                                         std::vector<::ccs::types::uint32> &dimension);

    bool GetVarValue(std::string varName,
                     ::ccs::types::AnyValue &var);

public:
    MathExpressionEngineExptrk();

    virtual ~MathExpressionEngineExptrk();

    //Just don use vars with name including I1234I (a number between two I´)
    virtual bool Compile(const ccs::types::char8 *expressionIn,
                         Workspace *workspace);

    virtual bool Execute();
};

}
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace sup {

namespace sequencer {

template<typename T>
MathExpressionEngineExptrk<T>::MathExpressionEngineExptrk() {
    inputCasts[0] = &fromBoolCast;
    inputCasts[1] = &fromChar8Cast;
    inputCasts[2] = &fromStringCast;
    inputCasts[3] = &fromInt8Cast;
    inputCasts[4] = &fromUint8Cast;
    inputCasts[5] = &fromInt16Cast;
    inputCasts[6] = &fromUint16Cast;
    inputCasts[7] = &fromInt32Cast;
    inputCasts[8] = &fromUint32Cast;
    inputCasts[9] = &fromInt64Cast;
    inputCasts[10] = &fromUint64Cast;
    inputCasts[11] = &fromFloat32Cast;
    inputCasts[12] = &fromFloat64Cast;
    inputCasts[13] = NULL;

    outputCasts[0] = &toBoolCast;
    outputCasts[1] = &toChar8Cast;
    outputCasts[2] = &toStringCast;
    outputCasts[3] = &toInt8Cast;
    outputCasts[4] = &toUint8Cast;
    outputCasts[5] = &toInt16Cast;
    outputCasts[6] = &toUint16Cast;
    outputCasts[7] = &toInt32Cast;
    outputCasts[8] = &toUint32Cast;
    outputCasts[9] = &toInt64Cast;
    outputCasts[10] = &toUint64Cast;
    outputCasts[11] = &toFloat32Cast;
    outputCasts[12] = &toFloat64Cast;
    outputCasts[13] = NULL;

    myWs = NULL;
}
template<typename T>
MathExpressionEngineExptrk<T>::~MathExpressionEngineExptrk() {
}

template<typename T>
std::string::size_type MathExpressionEngineExptrk<T>::CheckForArray(std::string varName,
                                                                    std::vector<::ccs::types::uint32> &dimension) {

    std::string::size_type prev_pos = 0;
    std::string::size_type pos = 0;
    std::string::size_type ret = varName.length();
    while ((pos = varName.find('I', pos)) != std::string::npos) {
        if (ret > pos) {
            ret = pos;
        }
        pos++;
        prev_pos = pos;
        log_info("MathExpressionEngineExptrk::GetVarValue - Found open array");
        if ((pos = varName.find('I', pos)) != std::string::npos) {
            std::string dimStr(varName.substr(prev_pos, pos - prev_pos));
            ::ccs::types::char8 *p;
            ::ccs::types::uint32 dim = std::strtol(dimStr.c_str(), &p, 10);
            //conversion failed
            if (*p != 0) {
                dimension.clear();
                ret = varName.length();
                break;
            }
            dimension.push_back(dim);
            log_info("MathExpressionEngineExptrk::GetVarValue - Added dimension %d", dim);
            pos++;
            prev_pos = pos;
        }
        else {
            break;
        }
    }
    return ret;
}

template<typename T>
bool MathExpressionEngineExptrk<T>::GetVarValue(std::string varName,
                                                ::ccs::types::AnyValue &var) {

    std::string::size_type prev_pos = 0;
    std::string::size_type pos = 0;
    pos = varName.find('.');
    std::string fVarNameT(varName.substr(0, pos));
    std::vector < ::ccs::types::uint32 > dimension;
    std::string::size_type arrPos = CheckForArray(fVarNameT, dimension);
    std::string fVarName(fVarNameT.substr(0, arrPos));
    log_info("MathExpressionEngineExptrk::GetVarValue - Considering %s", varName.c_str());

    ::ccs::types::AnyValue fullVar;
    log_info("MathExpressionEngineExptrk::GetVarValue - Workspace var is: %s", fVarName.c_str());
    bool ret = myWs->GetValue(fVarName, fullVar);

    ::ccs::base::SharedReference<const ::ccs::types::AnyType> myType = fullVar.GetType();
    ccs::types::uint32 offset = 0u;

    for (::ccs::types::uint32 i = 0u; (i < dimension.size()) && ret; i++) {
        log_info("MathExpressionEngineExptrk::GetVarValue - var %s dimension[%d]=%d", fVarName.c_str(), i, dimension[i]);

        ::ccs::base::SharedReference<const ::ccs::types::ArrayType> myArrayType = myType;
        ret = myArrayType.IsValid();
        if (ret) {
            offset += myArrayType->GetElementOffset(dimension[i]);
            myType = myArrayType->GetElementType();
        }
    }

    if (ret && (pos != std::string::npos)) {

        ret = myType.IsValid();
        //if not done, keep searching next field
        //in this case the type must be compound
        while ((pos != std::string::npos) && (ret)) {
            pos++;
            prev_pos = pos;

            pos = varName.find('.', pos);
            std::string fieldNameT(varName.substr(prev_pos, pos - prev_pos));
            dimension.clear();
            arrPos = CheckForArray(fieldNameT, dimension);
            std::string fieldName(fieldNameT.substr(0, arrPos));
            log_info("MathExpressionEngineExptrk::GetVarValue - FieldName %s", fieldName.c_str());

            ::ccs::base::SharedReference<const ::ccs::types::CompoundType> myCompType = myType;
            ret = myCompType.IsValid();
            if (ret) {
                offset += myCompType->GetAttributeOffset(fieldName.c_str());
                log_info("MathExpressionEngineExptrk::GetVarValue - var %s new offset=%d", fieldName.c_str(), offset);
                myType = myCompType->GetAttributeType(fieldName.c_str());
                ret = myType.IsValid();
                if (ret) {

                    //if dimension is not empty go to browse the array
                    for (::ccs::types::uint32 i = 0u; (i < dimension.size()) && ret; i++) {
                        log_info("MathExpressionEngineExptrk::GetVarValue - var %s dimension[%d]=%d", fieldName.c_str(), i, dimension[i]);
                        ::ccs::base::SharedReference<const ::ccs::types::ArrayType> myArrayType = myType;
                        ret = myArrayType.IsValid();
                        if (ret) {
                            offset += myArrayType->GetElementOffset(dimension[i]);
                            log_info("MathExpressionEngineExptrk::GetVarValue - var %s new offset=%d", fieldName.c_str(), offset);
                            myType = myArrayType->GetElementType();
                        }
                    }
                }
                else {
                    log_info("MathExpressionEngineExptrk::GetVarValue - Failed %s.GetType()", fieldName.c_str());
                }
            }
            else {
                log_info("MathExpressionEngineExptrk::GetVarValue - %s is not a Compound Type", myType->GetName());
            }
        }
    }
    if (ret) {
        ::ccs::types::uint8 *srcMem = reinterpret_cast<::ccs::types::uint8*>(fullVar.GetInstance());
        var = ::ccs::types::AnyValue(myType);
        ::ccs::types::uint32 size = myType->GetSize();
        ::ccs::types::uint8 *dstMem = reinterpret_cast<::ccs::types::uint8*>(var.GetInstance());
        memcpy(dstMem, &srcMem[offset], size);
    }
    return ret;
}

template<typename T>
bool MathExpressionEngineExptrk<T>::Compile(const ccs::types::char8 *expressionIn,
                                            Workspace *workspace) {

    myWs = workspace;
    ::ccs::types::boolean ret = (workspace != NULL);
    if (ret) {
        //get var names from expression
        std::string exprStr = expressionIn;
        std::replace(exprStr.begin(), exprStr.end(), '[', 'I');
        std::replace(exprStr.begin(), exprStr.end(), ']', 'I');

        ret = exprtk::collect_variables(exprStr.c_str(), varNames);
        if (ret) {
            //get the variable number of variables to preallocate the memory
            ::ccs::types::uint32 numberOfVars = varNames.size();
            varMem.resize(numberOfVars);
            varInCast.resize(numberOfVars);
            varOutCast.resize(numberOfVars);
            for (::ccs::types::uint32 i = 0u; (i < numberOfVars) && (ret); i++) {
                ::ccs::types::AnyValue var;

                ret = GetVarValue(varNames[i], var);

                if (ret) {

                    ::ccs::base::SharedReference<const ::ccs::types::AnyType> varTypeRef = var.GetType();
                    ret = varTypeRef.IsValid();
                    if (ret) {
                        //use arrays to be generic
                        ::ccs::types::uint32 varSize = (var.GetSize() / varTypeRef->GetSize());
                        varMem[i].resize(varSize);
                        ret = symbolTable.add_vector(varNames[i], varMem[i]);

                        if (ret) {
                            varInCast[i] = NULL;
                            varOutCast[i] = NULL;

                            //register the input cast function
                            ::ccs::types::uint8 typeId = static_cast<::ccs::types::uint8>(::ccs::types::GetIdentifier(varTypeRef->GetName()));
                            ret = (typeId < 13u);
                            if (ret) {
                                varInCast[i] = inputCasts[typeId];
                            }
                            else {
                                log_error("MathExpressionEngineExptrk::Compile Failed GetIdentifier of %s", varTypeRef->GetName());
                            }
                        }
                        else {
                            log_error("MathExpressionEngineExptrk::Compile Failed symbolTable.add_vector(%s)", varNames[i].c_str());
                        }
                    }
                    else {
                        log_error("MathExpressionEngineExptrk::Compile Failed GetType of %s", varNames[i].c_str());
                    }
                }
                else {
                    log_error("MathExpressionEngineExptrk::Compile Failed workspace->GetValue(%s)", varNames[i].c_str());
                }
            }
        }
        else {
            log_error("MathExpressionEngineExptrk::Compile Failed exprtk::collect_variables of %s", expressionIn);
        }

        if (ret) {
            //compile and get the assignments (outputs)
            std::deque<typename exprtk::parser<T>::dependent_entity_collector::symbol_t> symbol_list;
            expressionParser.dec().collect_assignments() = true;
            expression.register_symbol_table(symbolTable);

            ret = expressionParser.compile(exprStr.c_str(), expression);
            if (ret) {
                expressionParser.dec().assignment_symbols(symbol_list);
                for (ccs::types::uint32 i = 0; (i < symbol_list.size()) && ret; i++) {

                    for (ccs::types::uint32 j = 0; (j < varNames.size()); j++) {
                        //when the output matches the variable, save the output cast
                        if (varNames[j] == symbol_list[i].first) {
                            ::ccs::types::AnyValue var;
                            ret = GetVarValue(varNames[j], var);
                            if (ret) {
                                ::ccs::base::SharedReference<const ::ccs::types::AnyType> varTypeRef = var.GetType();
                                ret = varTypeRef.IsValid();
                                if (ret) {
                                    ::ccs::types::uint8 typeId = static_cast<::ccs::types::uint8>(::ccs::types::GetIdentifier(varTypeRef->GetName()));
                                    ret = (typeId < 13u);
                                    if (ret) {
                                        //for the output register the output cast function
                                        varOutCast[j] = outputCasts[typeId];
                                    }
                                    else {
                                        log_error("MathExpressionEngineExptrk::Compile Failed GetIdentifier of %s", varTypeRef->GetName());
                                    }
                                }
                                else {
                                    log_error("MathExpressionEngineExptrk::Compile Failed GetType of %s", varNames[j].c_str());
                                }
                            }
                            else {
                                log_error("MathExpressionEngineExptrk::Compile Failed workspace->GetValue(%s)", varNames[j].c_str());
                            }
                        }
                    }
                }

            }
            else {

                // Print a log of each detected error.
                for (::ccs::types::uint32 i = 0; i < expressionParser.error_count(); ++i) {
                    exprtk::parser_error::type error = expressionParser.get_error(i);
                    exprtk::parser_error::update_error(error, expressionIn);

                    log_error("Error[%i]. Position: row %i, col %i. Type: %s. Description: %s\n", i, static_cast<::ccs::types::uint32>(error.line_no),
                              static_cast<::ccs::types::uint32>(error.column_no), exprtk::parser_error::to_str(error.mode).c_str(), error.diagnostic.c_str());

                }

            }
        }
    }

    return ret;
}

template<typename T>
bool MathExpressionEngineExptrk<T>::Execute() {

    bool ret = (myWs != NULL);

    if (ret) {
        ::ccs::types::uint32 nVars = varMem.size();

        for (::ccs::types::uint32 i = 0u; i < nVars; i++) {
            //do it for each element
            if (varInCast[i] != NULL) {
                ::ccs::types::uint32 varSize = varMem[i].size();
                ::ccs::types::AnyValue valT;
                ret = GetVarValue(varNames[i], valT);
                if (ret) {
                    void *inPtr = valT.GetInstance();
                    for (::ccs::types::uint32 j = 0u; j < varSize; j++) {
                        //todo workspace lock here (or better lock single vars)
                        varInCast[i]->Do(inPtr, &varMem[i][0], j);
                    }
                }
            }
        }

        expression.value();

        for (::ccs::types::uint32 i = 0u; i < nVars; i++) {
            //do it for each element
            if (varOutCast[i] != NULL) {
                ::ccs::types::uint32 varSize = varMem[i].size();
                ::ccs::types::AnyValue valT;
                myWs->GetValue(varNames[i], valT);
                void *outPtr = valT.GetInstance();
                for (::ccs::types::uint32 j = 0u; j < varSize; j++) {
                    //todo workspace lock here (or better lock single vars)
                    varOutCast[i]->Do(&varMem[i][0], outPtr, j);
                }
                myWs->SetValue(varNames[i], valT);
            }
        }
    }
    return ret;
}

BasicCastI::BasicCastI() {

}

BasicCastI::~BasicCastI() {

}

template<typename Tin, typename Tout>
BasicCast<Tin, Tout>::BasicCast() {

}

template<typename Tin, typename Tout>
BasicCast<Tin, Tout>::~BasicCast() {

}

template<typename Tin, typename Tout>
void BasicCast<Tin, Tout>::Do(void *in,
                              void *out,
                              ccs::types::uint32 idx) {
    Tin input = (reinterpret_cast<Tin*>(in))[idx];
    (reinterpret_cast<Tout*>(out))[idx] = static_cast<Tout>(input);

}

}
}

#endif /* MATHEXPRESSIONENGINEEXPTRK_H_ */

