# Copyright 2015 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'refinery',
      'type': 'none',
      'dependencies': [
        'analyzers/analyzers.gyp:*',
        'core/core.gyp:*',
        'process_state/process_state.gyp:*',
        'minidump/minidump.gyp:*',
        'validators/validators.gyp:*',
      ],
    },
    {
      'target_name': 'refinery_unittest_utils',
      'type': 'static_library',
      'sources': [
        'unittest_util.cc',
        'unittest_util.h',
      ],
      'dependencies': [
        'process_state/process_state.gyp:process_state_lib',
        '<(src)/syzygy/core/core.gyp:core_unittest_utils',
        '<(src)/testing/gtest.gyp:gtest',
      ],
    },
    {
      'target_name': 'symsrv_dll_copy',
      'type': 'none',
      'msvs_cygwin_shell': 0,
      'copies': [
        {
          'destination': '<(PRODUCT_DIR)',
          'files': [
            '<(src)/third_party/debugging_tools/files/SymSrv.dll',
          ],
        },
      ],
    },
    {
      'target_name': 'refinery_unittests',
      'type': 'executable',
      'sources': [
        'analyzers/analysis_runner_unittest.cc',
        'analyzers/exception_analyzer_unittest.cc',
        'analyzers/memory_analyzer_unittest.cc',
        'analyzers/module_analyzer_unittest.cc',
        'analyzers/stack_analyzer_unittest.cc',
        'analyzers/thread_analyzer_unittest.cc',
        'analyzers/unloaded_module_analyzer_unittest.cc',
        'core/address_unittest.cc',
        'core/addressed_data_unittest.cc',
        'process_state/process_state_unittest.cc',
        'minidump/minidump_unittest.cc',
        'types/type_unittest.cc',
        'types/type_repository_unittest.cc',
        'types/typed_data_unittest.cc',
        'types/dia_crawler_unittest.cc',
        'types/pdb_crawler_unittest.cc',
        'validators/exception_handler_validator_unittest.cc',
        '<(src)/syzygy/testing/run_all_unittests.cc',
      ],
      'dependencies': [
        'analyzers/analyzers.gyp:analyzers_lib',
        'core/core.gyp:refinery_core_lib',
        'minidump/minidump.gyp:minidump_lib',
        'process_state/process_state.gyp:process_state_lib',
        'refinery_unittest_utils',
        'symsrv_dll_copy',
        'types/types.gyp:types_lib',
        'validators/validators.gyp:validators_lib',
        '<(src)/base/base.gyp:base',
        '<(src)/base/base.gyp:test_support_base',
        '<(src)/testing/gmock.gyp:gmock',
        '<(src)/testing/gtest.gyp:gtest',
       ],
    },
    {
      'target_name': 'run_refinery',
      'type': 'executable',
      'sources': [
        'run_refinery_main.cc',
      ],
      'dependencies': [
        'analyzers/analyzers.gyp:analyzers_lib',
        'core/core.gyp:refinery_core_lib',
        'minidump/minidump.gyp:minidump_lib',
        'process_state/process_state.gyp:process_state_lib',
        'types/types.gyp:types_lib',
        'validators/validators.gyp:validators_lib',
        '<(src)/base/base.gyp:base',
      ],
    },
  ]
}
