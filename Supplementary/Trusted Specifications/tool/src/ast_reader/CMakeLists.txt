set(LLVM_LINK_COMPONENTS support)

add_clang_executable(astreader
  ASTReader.cpp
  )
target_link_libraries(astreader
  clangTooling
  clangBasic
  clangASTMatchers
  )
