#  This file is part of f1x.
#  Copyright (C) 2016  Sergey Mechtaev, Gao Xiang, Shin Hwei Tan, Abhik Roychoudhury
#
#  f1x is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

FROM ubuntu:22.04
LABEL MAINTAINER="mirchevmartin2203@gmail.com"
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get upgrade -y && apt-get autoremove -y

RUN apt-get install -y build-essential make cmake ninja-build wget python2.7

RUN apt-get install -y llvm-15* clang-15 bear libear libclang-15-dev zlib* nano rapidjson-dev libboost-filesystem-dev libboost-program-options-dev libboost-log-dev
