# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/sergio/ESP32_IDF/esp-idf/components/bootloader/subproject"
  "/home/sergio/ESP32_IDF/examples-do-for-me/Buttons/build/bootloader"
  "/home/sergio/ESP32_IDF/examples-do-for-me/Buttons/build/bootloader-prefix"
  "/home/sergio/ESP32_IDF/examples-do-for-me/Buttons/build/bootloader-prefix/tmp"
  "/home/sergio/ESP32_IDF/examples-do-for-me/Buttons/build/bootloader-prefix/src/bootloader-stamp"
  "/home/sergio/ESP32_IDF/examples-do-for-me/Buttons/build/bootloader-prefix/src"
  "/home/sergio/ESP32_IDF/examples-do-for-me/Buttons/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/sergio/ESP32_IDF/examples-do-for-me/Buttons/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
