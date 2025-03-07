/*
 Copyright 2019 Alain Dargelas

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */
#include "Surelog/Design/Statement.h"

/*
 * File:   Statement.cpp
 * Author: alain
 *
 * Created on May 25, 2019, 11:34 AM
 */

#include <string_view>
#include <vector>

#include "Surelog/Common/NodeId.h"
#include "Surelog/Design/FileContent.h"
#include "Surelog/SourceCompile/VObjectTypes.h"

namespace SURELOG {

std::string_view SubRoutineCallStmt::getVarName(NodeId base_name) const {
  const FileContent* const fC = getFileContent();
  switch (fC->Type(base_name)) {
    case VObjectType::paSuper_keyword:
    case VObjectType::paThis_dot_super:
    case VObjectType::paSuper_dot_new:
      return "super";
    case VObjectType::paThis_keyword:
      return "this";
    case VObjectType::slStringConst:
      return fC->SymName(base_name);
    default:
      return "UNKNOWN_VAR_NAME";
  }
}

std::vector<std::string_view> SubRoutineCallStmt::getVarChainNames() const {
  std::vector<std::string_view> result;
  result.reserve(m_var_chain.size());
  for (auto node : m_var_chain) {
    result.emplace_back(getVarName(node));
  }
  return result;
}

}  // namespace SURELOG
