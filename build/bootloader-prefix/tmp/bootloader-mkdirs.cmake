# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/batera/esp/v5.2.2/esp-idf/components/bootloader/subproject"
  "/home/batera/Documents/project/porto_nacional/telemetry_idf/sample_project/build/bootloader"
  "/home/batera/Documents/project/porto_nacional/telemetry_idf/sample_project/build/bootloader-prefix"
  "/home/batera/Documents/project/porto_nacional/telemetry_idf/sample_project/build/bootloader-prefix/tmp"
  "/home/batera/Documents/project/porto_nacional/telemetry_idf/sample_project/build/bootloader-prefix/src/bootloader-stamp"
  "/home/batera/Documents/project/porto_nacional/telemetry_idf/sample_project/build/bootloader-prefix/src"
  "/home/batera/Documents/project/porto_nacional/telemetry_idf/sample_project/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/batera/Documents/project/porto_nacional/telemetry_idf/sample_project/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/batera/Documents/project/porto_nacional/telemetry_idf/sample_project/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
