# MIT License
#
# Copyright (c) 2024 Kevin Traini
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
include_guard()

function(filc_package)
    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VENDOR "filc developers")
    set(CPACK_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")
    set(CPACK_PACKAGE_CONTACT "Kevin Traini <kevin@ktraini.com>")
    set(CPACK_PACKAGE_DESCRIPTION "Fil compiler")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Fil compiler")
    set(CPACK_PACKAGE_HOMEPAGE_URL "https://fil.ktraini.com")
    set(CPACK_PACKAGE_CHECKSUM "SHA512")
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.md")
    set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")

    # RPM package
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
    set(CPACK_RPM_PACKAGE_DESCRIPTION "Fil compiler")
    set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr" "/usr/bin")
    set(CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_BINARY_DIR}/changelog")

    include(CPack)
endfunction()
