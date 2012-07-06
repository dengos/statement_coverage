GTEST_DIR=/home/dengos/.gtest-1.6.0

test_path_record:
	g++ -I${GTEST_DIR}/include/ gtest_main.cc test_path_record.cc ../src/path_record.cc libgtest.a -lpthread -D GTEST

test_instrumentor:
	g++ -I${GTEST_DIR}/include/ gtest_main.cc test_instrumentor.cc ../src/instrumentor.cc libgtest.a -lpthread -lclang -D GTEST 

test_do_inject:
	g++ -g -I${GTEST_DIR}/include/ -I../src/ gtest_main.cc test_do_inject.cc ../src/instrumentor.cc libgtest.a -lpthread -lclang -D GTEST 

test_inject_expr:
	g++ -g -I${GTEST_DIR}/include/ -I../src/ gtest_main.cc test_inject_expr.cc ../src/instrumentor.cc libgtest.a -lpthread -lclang -D GTEST

test_template:
	g++ -g -I${GTEST_DIR}/include/ -I../src/ gtest_main.cc test_template.cc ../src/instrumentor.cc libgtest.a -lpthread -lclang -D GTEST

test_class:
	g++ -g -I${GTEST_DIR}/include/ -I../src/ gtest_main.cc test_class.cc ../src/instrumentor.cc libgtest.a -lpthread -lclang -D GTEST







clean:
	rm path_record_*
	rm a.out
