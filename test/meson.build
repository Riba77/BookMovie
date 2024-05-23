
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')


tests_src = [
  'cinemaplex_test.cpp',
] 

# gtest_dep = dependency('gtest', main : true, required : false)
e = executable('testprog', 
    tests_src, 
    dependencies : [ gmock_dep, gtest_dep, gtest_main_dep ],
    include_directories : cinemaplex_includes,
    link_with : [ cinemaplex_lib ],
    build_by_default : false,
    install : false
    )
