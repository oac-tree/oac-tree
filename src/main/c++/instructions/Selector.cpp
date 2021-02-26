/**
 * @file Selector.cpp
 * @brief Source file for class Selector
 * @date Feb 25, 2021 TODO Verify the value and format of the date
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

 * @details This source file contains the definition of all the methods for
 * the class Selector (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Selector.h"
#include "Workspace.h"

#include <common/AnyValue.h>
#include <common/AnyType.h>
#include <common/log-api.h>
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace sup {

namespace sequencer {
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

const std::string Selector::Type = "Selector";

Selector::Selector() :
		CompoundInstruction(Type)
{
	numberOfElements = 1u;
	elementSize = 0u;
	isMask = false;
}

Selector::~Selector()
{
//Auto-generated destructor stub for Selector

//TODO Verify if manual additions are needed here
}

bool Selector::Setup(Workspace *ws)
{
	bool ret = CompoundInstruction::Setup(ws);
	if (ret)
	{
		ret = HasAttribute("var_name");
		if (ret)
		{
			varName = GetAttribute("var_name");
			::ccs::types::AnyValue _val;
			ret = ws->GetValue(varName, _val);
			if (ret)
			{
				elementSize = _val.GetSize();
				isMask = HasAttribute("is_mask");
				if (isMask)
				{
					isMask = (GetAttribute("is_mask") == "true");
				}
				//check if array
				if (!isMask)
				{
					::ccs::base::SharedReference<const ::ccs::types::ArrayType> myArrayType = _val.GetType();
					if (myArrayType.IsValid())
					{
						numberOfElements = myArrayType->GetElementNumber();
						elementSize = myArrayType->GetElementType()->GetSize();
					}
					//they must be arrays of uint32 or positive int32
					ret = (elementSize <= 4u);
					if (!ret)
					{
						log_error("Selector::Setup - element size must be <= 4");
					}
				}
			}
		}
		else
		{
			log_error("Selector::Setup - No attribute var_name found");
		}

		log_info("Selector::Setup - With var_name=%s numberOfElements=%u elementSize=%u isMask=%u", varName.c_str(),
				numberOfElements, elementSize, isMask);
	}
	return ret;
}

ExecutionStatus Selector::ExecuteSingleImpl(UserInterface *ui, Workspace *ws)
{
	ExecutionStatus child_status = ExecutionStatus::SUCCESS;
	::ccs::types::AnyValue _val;
	ws->GetValue(varName, _val);
	::ccs::types::uint8 *valPtr = reinterpret_cast<::ccs::types::uint8*>(_val.GetInstance());

	if (isMask)
	{
		::ccs::types::uint32 nElems = (elementSize * 8u);
		bool exit = false;
		//generic...can consider very big elements
		while ((nElems > 0u) && (!exit))
		{
			::ccs::types::uint32 remained = (nElems > 64u) ? (64u) : (nElems);
			::ccs::types::uint64 value = 0u;
			memcpy(&value, valPtr, (remained / 8u));

			for (::ccs::types::uint32 i = 0u; (i < remained) && (!exit); i++)
			{
				if (((value >> i) & (0x1u)))
				{
					child_status = ExecuteChild(i, ui, ws);
					//continue only if success
					if (child_status != ExecutionStatus::SUCCESS)
					{
						child_status = _children[i]->GetStatus();
						exit = true;
					}
				}
			}
			nElems -= remained;
			valPtr += 8u;
		}
	}
	else
	{
		for (::ccs::types::uint32 i = 0u; i < numberOfElements; i++)
		{
			::ccs::types::uint32 value = 0u;

			memcpy(&value, valPtr, elementSize);

			child_status = ExecuteChild(value, ui, ws);
			//continue only if success
			if (child_status != ExecutionStatus::SUCCESS)
			{
				child_status = _children[value]->GetStatus();
				break;
			}
			valPtr += elementSize;
		}
	}
	return child_status;
}

ExecutionStatus Selector::ExecuteChild(::ccs::types::uint32 idx, UserInterface *ui, Workspace *ws)
{
	ExecutionStatus child_status = ExecutionStatus::SUCCESS;

	if (idx < _children.size())
	{
		child_status = _children[idx]->GetStatus();

		if (NeedsExecute(child_status))
		{
			auto childName = _children[idx]->GetName();
			log_info("Selector::ExecuteChild - Executing child[%u]=%s", idx, childName.c_str());

			_children[idx]->ExecuteSingle(ui, ws);
			child_status = ExecutionStatus::NOT_FINISHED;
		}
	}
	else
	{
		log_warning("Status Selector::ExecuteSingleImpl - child[%u] not executed because exceeding children size (%u)", idx,
				_children.size());
	}
	return child_status;
}

}

}
