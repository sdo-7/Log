cmake_minimum_required(VERSION 3.28)

project(log LANGUAGES CXX)

add_executable(log
  src/main.cpp
  src/Log.cpp
  src/Record.cpp
  src/Request.cpp
)

set_property(TARGET log PROPERTY CXX_STANDARD 20)
set_property(TARGET log PROPERTY CXX_STANDARD_REQUIRED ON)

enable_testing()

function(do_test target testName longestCount file result)
  add_test(NAME ${testName} COMMAND ${target} ${longestCount} ${file})
  set_tests_properties(${testName} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endfunction()

set(noErrorResult "^2024-01-20T22:19:24,200000[\t ]+INFO[\t ]+Request started [\(]Name: Lookup, TraceId: 837bcffb-f58cf5-3fe03e8-43766f35[\)][\r\n\t ]+2024-01-20T22:19:24,200001[\t ]+INFO[\t ]+Some text [\(]Name: Lookup, TraceId: 837bcffb-f58cf5-3fe03e8-43766f35[\)][\r\n\t ]+2024-01-20T22:19:24,200001[\t ]+INFO[\t ]+Some text [\(]Name: Lookup, TraceId: 837bcffb-f58cf5-3fe03e8-43766f35[\)][\r\n\t ]+2024-01-20T22:19:24,200002[\t ]+INFO[\t ]+Request started [\(]Name: Lookup, TraceId: 837bcffb-f58cf5-3fe03e8-43766f36[\)][\r\n\t ]+2024-01-20T22:19:26,200002[\t ]+INFO[\t ]+Another text [\(]Name: Lookup, TraceId: 837bcffb-f58cf5-3fe03e8-43766f36[\)][\r\n\t ]+2024-01-20T22:19:26,200003[\t ]+INFO[\t ]+Request failed [\(]Error: some error, TraceId: 837bcffb-f58cf5-3fe03e8-43766f36[\)][\r\n\t ]+2024-01-20T22:19:22,907403[\t ]+INFO[\t ]+Request started [\(]Name: Insert, TraceId: e69d733f-e72e8c7-3fe03e8-17ac5302[\)][\r\n\t ]+2024-01-20T22:19:23,766088[\t ]+INFO[\t ]+Parsing headers [\(]TraceId: e69d733f-e72e8c7-3fe03e8-17ac5302[\)][\r\n\t ]+2024-01-20T22:19:23,813662[\t ]+INFO[\t ]+Request completed [\(]TraceId: e69d733f-e72e8c7-3fe03e8-17ac5302[\)][\r\n\t ]+$")

do_test(log "noError" 3 "../test/test.log" ${noErrorResult})
do_test(log "wrongFile" 3 "../test/notexists" "^could not open the file\n$")
do_test(log "doubleStart" 3 "../test/error1.log" "^error on line 2: the request with this traceId is already started\n$")
do_test(log "noStart" 3 "../test/error2.log" "^error on line 1: the request with this traceId is not started\n$")
do_test(log "doubleEnd" 3 "../test/error3.log" "^error on line 3: the request is already ended\n$")
do_test(log "lessTime" 3 "../test/error4.log" "^error on line 3: time field value of the record is less than previous one\n$")
do_test(log "wrongTimeFormat" 3 "../test/error5.log" "^error on line 1: could not parse time field\n$")
do_test(log "wrongIdFormat" 3 "../test/error6.log" "^error on line 1: could not parse traceId field\n$")
