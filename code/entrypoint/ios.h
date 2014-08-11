#pragma once

bool EntryPoint_initLog(const char* path);
bool EntryPoint_init(const char* storagePath, const char* homeDirPath, void* renderIniter, void* myRenderView);
void EntryPoint_deinit();
bool EntryPoint_step();
void EntryPoint_stepFinish();
void EntryPoint_onTouch(unsigned int index, float x, float y, bool up, bool down);