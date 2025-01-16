# XCFG  

Custom configuration format.  

### Config format
```
section [attributes] {
    field: value,
    subsection [attribute=value] {
        field: value
        list: {element1, element2, element3}
    }
}
```

### API
Base type for config is `xcfg::Value`.
Values can store an object (key-value pair) or a string value.  
Values can also be empty or contain an expression.   
  
A string value of a field can be accessed using `xcfg::Value::string()`.  
Attributes are accessible through `xcfg::Value::attrs()`.  
Subsections can be accessed using `xcfg::Value::items()`.  
Also it is possible to get nested value using `xcfg::Value::get(const std::string& path)`, e.g. `root.get("section.subsection.value")`, which itself will return `xcfg::Value`.
Type of value can be checked using `xcfg::Value::isString()` and `xcfg::Value::isObject()`.  
  
String can be parsed into a `xcfg::Value` using `xcfg::parse(const std::string& src)`.  
As was mentioned above - a value can also be an expression.  
Expressions allow users to include other files, define constants and use them later on.  
Expressions aren't processed automatically, to process all expressions in a value `xcfg::process(Value& root, Value& value, Env& env)` is used.  
`xcfg::Env` contains include paths and defines.  

Example of expressions:  
```
${include another_config_file.xcfg}
${define CONSTANT 42}
section {
    subsection1 {
        value: ${CONSTANT}
    }
    subsection2 {
        ${insert section.subsection1}
    }
}
```
`include` takes 1 parameter - path to file, which will be parsed and inserted in place of include directive.  
`define` takes 2 parameters - name and value of a constant.  
`insert` takes 1 parameter - comma separated path of an element relative to root, this element will be inserted in place.  
Any other words encased in `${}` will be treated as a constant name, and will be inserted in place.


### How to install  
#### Prerequisites:  
 - Unix like system (WSL works too)
 - `gcc` or `clang` with `c++17` support
 - `cmake`

#### To build:  
 - `cmake -S . -B build`
 - `cmake --build build`