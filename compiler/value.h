#pragma once

enum ValueType {
  i32,
  i64,
  f32,
  f64
};

struct Value {
  ValueType type;

  float number;

  Value(float val) : type(ValueType::f64) {
    number = val;
  }
};
