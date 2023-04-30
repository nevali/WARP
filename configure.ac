
## Copyright 2023 Mo McRoberts.
##
##  Licensed under the Apache License, Version 2.0 (the "License");
##  you may not use this file except in compliance with the License.
##  You may obtain a copy of the License at
##
##      http://www.apache.org/licenses/LICENSE-2.0
##
##  Unless required by applicable law or agreed to in writing, software
##  distributed under the License is distributed on an "AS IS" BASIS,
##  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
##  See the License for the specific language governing permissions and
##  limitations under the License.

## BuildScripts/get-version.sh generates a version number from git tags
AC_INIT([WARP],[0.0])
## M4 macros are in a subdirectory named "M4"
AC_CONFIG_MACRO_DIR([M4])
## Build scripts are in a subdirectory named "BuildScripts"
AC_CONFIG_AUX_DIR([BuildScripts])

AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_MAINTAINER_MODE

AS_VAR_APPEND([AM_CPPFLAGS],[" -W -Wall"])
AS_VAR_APPEND([AM_CXXFLAGS],[" -O0 -g"])

AS_VAR_APPEND([AM_CPPFLAGS],[" -I\${top_srcdir}/PrivateHeaders"])
AS_VAR_APPEND([AM_CPPFLAGS],[" -I\${top_srcdir}/Headers"])

AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CXXFLAGS])

AC_PROG_CXX

LT_INIT

AC_CONFIG_FILES([
Makefile
Sources/Makefile
Sources/Flux/Makefile
Sources/Beacon/Makefile
Samples/Makefile
])

AC_OUTPUT
