#pragma once


namespace age {

class Struct {};
class Layout {};
class Function {};

class ShaderGen {
    void addStruct();
    void addFunction();
    void addLayout();
    void appendToMain();
};

/*

descriptor:
    buffer / texture,
    stage,
    type

*/

} // namespace age
