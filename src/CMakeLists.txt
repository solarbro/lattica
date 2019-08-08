set(LATTICA_SRC
"generate.c"
)

set(LATTICA_LIB lattica)

add_library(${LATTICA_LIB} STATIC ${LATTICA_SRC})

target_include_directories(${LATTICA_LIB} PUBLIC ${PROJECT_SOURCE_DIR}/include/)
