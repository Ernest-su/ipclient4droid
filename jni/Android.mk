# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
APP_STL  :=  stlprot_static 
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := ipclient
LOCAL_SRC_FILES := com_molei_ipclient4droid_IPClient.cpp ipClientPack.cpp MD5.cpp
LOCAL_C_INCLUDES := /media/D/android-ndk-r8e/sources/cxx-stl/stlport/stlport LOCAL_PATH
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib  -llog \
 /media/D/android-ndk-r8e/sources/cxx-stl/stlport/libs/armeabi/libstlport_static.a
include $(BUILD_SHARED_LIBRARY)
