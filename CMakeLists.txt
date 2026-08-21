cmake_minimum_required(VERSION 3.10)
project(MSPSO_APM)

# Configura o padrão C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Procura e habilita o OpenMP
find_package(OpenMP REQUIRED)

# Todos os arquivos fonte que implementamos
set(SOURCES
    src/main.cpp
    src/core/mspso.cpp
    src/core/apm.cpp
    src/problems/SpringProblem.cpp
    src/problems/ReducerProblem.cpp
    src/problems/WeldedBeamProblem.cpp
    src/problems/PressureVesselProblem.cpp
)

# Adiciona o executável
add_executable(mspso_apm ${SOURCES})

# Linka o OpenMP ao nosso executável
if(OpenMP_CXX_FOUND)
    target_link_libraries(mspso_apm PUBLIC OpenMP::OpenMP_CXX)
endif()