#make both dependencies into one array
deps = [
    dependency('unit-system', required: true),
    dependency('gtest', required: true, fallback:['gtest','gtest_dep']),
]
    
tests = [
    'unit_test',
    'base_units_test',
    'common_test',
]

test_incdir = include_directories('.')

#create all the tests
foreach test_obj : tests
    exe = executable(
        test_obj, 
        test_obj + '.cpp',
        dependencies: deps,
        include_directories : test_incdir,
    )
    test(
        test_obj + ' Test',
        exe,
        protocol: 'gtest',
    )
endforeach
