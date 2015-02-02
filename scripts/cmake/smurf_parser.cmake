cmake_policy(PUSH)
cmake_minimum_required(VERSION 2.6)

function(define_module_info)
  execute_process(COMMAND git rev-parse
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE under_git_control
    ERROR_QUIET)
  if(NOT under_git_control)
    execute_process(COMMAND git rev-parse HEAD 
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      OUTPUT_VARIABLE git_hash
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND git diff-index --quiet HEAD 
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      RESULT_VARIABLE git_has_local_changes
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND git remote -v
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      OUTPUT_VARIABLE git_src
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REGEX REPLACE "	(([^ ]|[:/])*) \\(fetch\\).*" "\\1" git_src ${git_src})
    if(git_has_local_changes)
      message(WARNING "git repository has local uncommitted changes!")
      set(git_revision "${git_hash}_localChanges")
    else(git_has_local_changes)
      set(git_revision ${git_hash})
    endif(git_has_local_changes)
    add_definitions("-DGIT_INFO" "-DGIT_INFO_REV=${git_revision}" "-DGIT_INFO_SRC=\"${git_src}\"")
  endif(NOT under_git_control)
endfunction(define_module_info)

cmake_policy(POP)
