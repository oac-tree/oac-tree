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

#ifndef SRC_MAIN_C___INSTRUCTIONS_THIRDPARTY_MATHEXPRESSIONENGINEEXPTRK_H_
#define SRC_MAIN_C___INSTRUCTIONS_THIRDPARTY_MATHEXPRESSIONENGINEEXPTRK_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "exprtk.hpp"
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "../MathExpressionEngineI.h"
#include "common/BasicTypes.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace sup {

namespace sequencer {

class BasicCastI {
    BasicCastI();
    virtual ~BasicCastI();
    virtual void Do(void *in,
                    void *out,
                    ccs::types::uint32 idx)=0;
};

template<typename Tin, typename Tout>
class BasicCast: public BasicCastI {
    BasicCast();
    virtual ~BasicCast();
    virtual void Do(void *in,
                    void *out,
                    ccs::types::uint32 idx);
};

template<typename T>
class MathExpressionEngineExptrk: public MathExpressionEngineI {

private:
    std::vector<std::vector<T> > varMem;
    std::vector<BasicCastI*> varInCast;
    std::vector<BasicCastI*> varOutCast;
    std::vector<void*> varPtr;
    uint64 numOfSignalVariables;

    exprtk::symbol_table<T> symbolTable; //<! Stores variables.
    exprtk::parser<T> expressionParser; //<! Parses the expressions.
    exprtk::expression<T> expression; //<! Stores one expression for each output signal declared in the configuration file.

    BasicCast<T, ccs::base::boolean> toBoolCast;
    BasicCast<T, ccs::base::char8> toChar8Cast;
    BasicCast<T, ccs::base::char8> toStringCast;
    BasicCast<T, ccs::base::int8> toInt8Cast;
    BasicCast<T, ccs::base::uint8> toUint8Cast;
    BasicCast<T, ccs::base::int16> toInt16Cast;
    BasicCast<T, ccs::base::uint16> toUint16Cast;
    BasicCast<T, ccs::base::int32> toInt32Cast;
    BasicCast<T, ccs::base::uint32> toUint32Cast;
    BasicCast<T, ccs::base::int64> toInt64Cast;
    BasicCast<T, ccs::base::uint64> toUint64Cast;
    BasicCast<T, ccs::base::float32> toFloat32Cast;
    BasicCast<T, ccs::base::float64> toFloat64Cast;

    BasicCast<ccs::base::boolean, T> fromBoolCast;
    BasicCast<ccs::base::char8, T> fromChar8Cast;
    BasicCast<ccs::base::char8, T> fromStringCast;
    BasicCast<ccs::base::int8, T> fromInt8Cast;
    BasicCast<ccs::base::uint8, T> fromUint8Cast;
    BasicCast<ccs::base::int16, T> fromInt16Cast;
    BasicCast<ccs::base::uint16, T> fromUint16Cast;
    BasicCast<ccs::base::int32, T> fromInt32Cast;
    BasicCast<ccs::base::uint32, T> fromUint32Cast;
    BasicCast<ccs::base::int64, T> fromInt64Cast;
    BasicCast<ccs::base::uint64, T> fromUint64Cast;
    BasicCast<ccs::base::float32, T> fromFloat32Cast;
    BasicCast<ccs::base::float64, T> fromFloat64Cast;

    BasicCastI *inputCasts[] = { fromBoolCast, fromChar8Cast, fromStringCast, fromInt8Cast, fromInt16Cast, fromUint16Cast, fromInt32Cast, fromUint32Cast,
            fromInt64Cast, fromUint64Cast, fromFloat32Cast, fromFloat64Cast };

    BasicCastI *outputCasts[] = { toBoolCast, toChar8Cast, toStringCast, toInt8Cast, toInt16Cast, toUint16Cast, toInt32Cast, toUint32Cast, toInt64Cast,
            toUint64Cast, toFloat32Cast, toFloat64Cast };

public:
    MathExpressionEngineExptrk();

    virtual ~MathExpressionEngineExptrk();

    virtual bool Compile(const char8 *expressionIn);

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

}
template<typename T>
MathExpressionEngineExptrk<T>::~MathExpressionEngineExptrk() {

}

template<typename T>
bool MathExpressionEngineExptrk<T>::Compile(const char8 *expressionIn,
                                            Workspace *workspace) {

    ::ccs::types::boolean ret = (workspace != NULL);
    if (ret) {
        vector < std::string > varNames;
        //get var names from expression
        ret = exprtk::collect_variables(expressionIn, varNames);
        if (ret) {
            ::ccs::types::uint32 numberOfVars = varNames.size();
            varMem.resize[numberOfVars];
            varInCast.resize[numberOfVars];
            varOutCast.resize[numberOfVars];
            varPtr.resize[numberOfVars];
            for (::ccs::types::uint32 i = 0u; (i < numberOfVars) && (ret); i++) {
                ::ccs::types::AnyValue varVal;

                //get the variable number of variables to preallocate the memory
                ret = workspace.GetValue(varNames[i], varVal);
                if (ret) {
                    ::ccs::types::uint32 varSize = varVal.GetSize();
                    varMem[i].resize(varSize);
                    ret = symbolTable.add_vector(varNames[i], varMem[i]);

                    if (ret) {
                        ::ccs::types::SharedReference < AnyType > varTypeRef = varVal.GetType();

                        ret = varTypeRef.IsValid();
                        if (ret) {
                            //todo control here
                            ::ccs::types::uint8 typeId = static_cast<::ccs::types::uint8>(GetIdentifier(varTypeRef->GetName()));
                            varInCast[i] = &inputCasts[typeId];
                            varPtr[i] = varVal.GetInstance();
                            varOutCast[i] = NULL;
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        std::deque < exprtk::parser<T>::dependent_entity_collector::symbol_t > symbol_list;
        expressionParser.dec().collect_assignments() = true;
        expression.register_symbol_table(symbolTable);
        ret = expressionParser.compile(expressionIn, expression);
        if (ret) {
            expressionParser.dec().assignment_symbols(symbol_list);
            for (ccs::type::uint32 i = 0; (i < symbol_list.size()) && ret; i++) {

                ::ccs::types::AnyValue varVal;
                ret = workspace.GetValue(symbol_list[i].first, varVal);
                if (ret) {
                    ::ccs::types::SharedReference < AnyType > varTypeRef = varVal.GetType();
                    ret = varTypeRef.IsValid();
                    if (ret) {
                        //todo control here
                        ::ccs::types::uint8 typeId = static_cast<::ccs::types::uint8>(GetIdentifier(varTypeRef->GetName()));
                        varOutCast[i] = &outputCasts[typeId];
                    }
                }
            }

        }
        else {
            //REPORT_ERROR(ErrorManagement::ParametersError, "Compilation of expression for output n. %i failed. Check the expression syntax.", sigIdx);

            // Print a log of each detected error.
            for (::ccs::types::uint32 i = 0; i < expressionParser.error_count(); ++i) {

                exprtk::parser_error::type error = expressionParser.get_error(i);

                exprtk::parser_error::update_error(error, expressionIn);

                /*REPORT_ERROR(ErrorManagement::Information, "Error[%i]. Position: row %i, col %i. Type: %s. Description: %s.", i,
                 static_cast<uint32>(error.line_no), static_cast<uint32>(error.column_no), exprtk::parser_error::to_str(error.mode).c_str(),
                 error.diagnostic.c_str());*/

            }

        }
    }
    return ret;
}

template<typename T>
bool MathExpressionEngineExptrk<T>::Execute() {
    ::ccs::types::uint32 nVars = varMem.size();

    for (::ccs::types::uint32 i = 0u; i < nVars; i++) {
        //do it for each element
        ::ccs::types::uint32 varSize = varMem[i].size();
        for (::ccs::types::uint32 j = 0u; j < varSize; j++) {
            //todo workspace lock here (or better lock single vars)
            if (varInCast[i] != NULL) {
                varInCast[i]->Do(varPtr[i], &varMem[i], j);
            }
        }
    }

    expression.value();

    for (::ccs::types::uint32 i = 0u; i < nVars; i++) {
        //do it for each element
        ::ccs::types::uint32 varSize = varMem[i].size();
        for (::ccs::types::uint32 j = 0u; j < varSize; j++) {
            //todo workspace lock here (or better lock single vars)
            if (varOutCast[i] != NULL) {
                varOutCast[i]->Do(&varMem[i], varPtr[i], j);
            }
        }
    }
    return true;
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
    Tin input = reinterpret_cast<Tin*>(in);
    (reinterpret_cast<Tout*>(out))[idx] = static_cast<Tout>(input[idx]);
}

}
}

#endif /* SRC_MAIN_C___INSTRUCTIONS_THIRDPARTY_MATHEXPRESSIONENGINEEXPTRK_H_ */

