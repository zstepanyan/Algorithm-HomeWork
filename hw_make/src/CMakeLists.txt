add_library(my_make_lib STATIC
    # core
    core/errors.cpp

    # parser (lexer + parser + ast)
    parser/lexer.cpp
    parser/parser.cpp

    # eval (variable table + future evaluator/functions)
    eval/variable_table.cpp

    # engine (rules + graph + executor)
    engine/rule_database.cpp
    engine/graph.cpp
    engine/executor.cpp

    # cli
    cli/cli.cpp
)
target_include_directories(my_make_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(my_make cli/main.cpp)
target_link_libraries(my_make PRIVATE my_make_lib)
