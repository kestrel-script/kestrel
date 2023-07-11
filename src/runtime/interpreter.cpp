#include "interpreter.hpp"

#include <ctype.h>
#include <iostream>
#include <vector>
#include <stack>

#include "log.hpp"
#include "value.hpp"
#include "function.hpp"
#include "opcodes.hpp"

#include "runtime/stack.hpp"
#include "core/core.hpp"

namespace kestrel {

void Interpreter::run(Module& module, Function& function) {
  LogLevel level = LogLevel::Debug;
  std::string tag = "interp";

  Stack<Value> stack;
  Stack<Frame> frames; // TODO move to coroutine?
  std::vector<Value> locals_i;
  locals_i.reserve(256); // TODO
  
  frames.push(Frame(function));

  std::vector<Value>* locals = &locals_i;
  Frame* frame = &frames.top();
  InstructionArray* instructions = &frame->function->instructions();
  int pc = (frame->pc);

#define RELOAD() \
  frame = &frames.top(); \
  instructions = &frame->function->instructions(); \
  pc = (frame->pc); \
  locals = &frame->locals;

  while (pc < instructions->size()) {
    Opcode opcode = (Opcode)instructions->readByte(pc++);
    Log(level, tag) << "opcode:" << (int)opcode;

    switch (opcode) {
    case Opcode::NoOP: {
      Log(level, tag) << "Nop";
      continue;
    }
    case Opcode::BranchFalse: {
      int offset = instructions->readShort(pc);
      // std::cout << "offset:" << offset << std::endl;
      pc += 2;
      if (stack.top().boolValue() == false) {
        pc += offset;
      }
      stack.pop();
      continue;
    }
    case Opcode::Add:
    case Opcode::Subtract:
    case Opcode::Equals: 
    case Opcode::LessThan:
    case Opcode::GreaterThan: {
      Value& left = stack[-2];
      Value& right = stack[-1];

      Value result;
      switch (opcode) {
        case Opcode::Add: {
          result = Value(left.intValue() + right.intValue());
          break;
        }
        case Opcode::Subtract: {
          result = Value(left.intValue() - right.intValue());
          break;
        }
        case Opcode::Equals: {
          result = Value(left.intValue() == right.intValue());
          break;
        }
        case Opcode::LessThan: {
          result = Value(left.intValue() < right.intValue());
          break;
        }
        case Opcode::GreaterThan: {
          result = Value(left.intValue() > right.intValue());
          break;
        }
      }
      
      stack.pop(2);
      stack.push(std::move(result));
      continue;
    }
    case Opcode::LoadInteger: {
      int value = instructions->readShort(pc);
      pc += 2;
      stack.push({value});
      continue;
    }
    case Opcode::LoadConstant: {
      int index = instructions->readShort(pc);
      pc += 2;
      Value& value = module.constants[index];
      // std::cout << "loading:" << value << std::endl;
      stack.push(value);
      continue;
    }
    case Opcode::LoadLocal: {
      int localIndex = instructions->readShort(pc);
      pc += 2;
      std::cout << "localIndex:" << localIndex << std::endl;
      Value& value = locals->at(localIndex);
      std::cout << "loading local :" << value << std::endl;
      stack.push(value);
      stack.inspect();
      continue;
    }
    // case Opcode::Load=
    case Opcode::Store: {
      int index = instructions->readShort(pc);
      pc += 2;
      (*locals)[index] = stack.pop();
      // std::cout << "storing " << (*locals)[index] << " at index:" << index << std::endl;
      continue;
    }
    case Opcode::LoadGlobal: {
      std::cout << "LoadGlobal" << std::endl;
      int index = instructions->readShort(pc);
      pc += 2;
      std::string& name = module.names[index];
      std::cout << "name:" << name << std::endl;

      if (module.hasGlobal(name)) {
        Value& val = module.getGlobal(name);
        std::cout << "LoadGlobal:" << name << " val:" << val << std::endl;
        stack.push(val);
      } else {
        std::cout << "Cannot find global:" << name << std::endl;
        return; // TODO
      }
      continue;
    }
    case Opcode::GetItem: {
      int index = instructions->readShort(pc);
      pc += 2;
      std::string& name = module.names[index];
      Value& tos = stack.top();
      core::Object* object;
      // Value& attr = object->getAttribute(name);
      
      // core::Object* item = object->getItem(name);
      // if (item == nullptr) {
        
      // }
      
      continue;
    }
    case Opcode::Dispatch: {
      int index = instructions->readShort(pc);
      pc += 2;
      int arity = instructions->readShort(pc);
      pc += 2;

      std::string name = module.names[index];
      std::cout << "Dispatch:" << module.names[index] << " : " << arity << std::endl;
      stack.inspect();
      
      std::vector<Value> args;
      int first = stack.size() - arity;

      // TODO
      for (int i = 0; i < arity; i++) {
        Value& arg = stack[first + i];
        args.push_back(stack[first + i]);
      }

      Value& value = stack[first - 1];
      std::cout << "metaClass:" << value.metaClass() << std::endl;

      MethodParameter params = {args};
      Value res = value.metaClass()->dispatch(value, name, params);
      std::cout << "res:" << res << std::endl;
      stack.pop(arity + 1);
      stack.push(res);

      // value
      // stack.pop();

      continue;
    }
    case Opcode::Call: {
      int arity = instructions->readShort(pc);
      pc += 2;

      // std::cout << "Call:" << arity << std::endl;
      
      int first = stack.size() - arity;
      Value& val = stack[first - 1];
      // std::cout << "val:" << val << std::endl;
      // // std::cout << "type:"
      std::shared_ptr<Function> function = val.functionValue();
      if (val.type() == ValueType::Class) {
        // TODO move to a function;
        std::cout << "class:" << val.metaClass() << std::endl;

        std::vector<Value> args;
        for (int i = 0; i < arity; i++) {
          Value& arg = stack[first + i];
          // std::cout << "arg:" << arg << std::endl;
          args.push_back(stack[first + i]);
        }
        MethodParameter mp = {args};
        // mp.args = args;
        Value object = val.metaClass()->construct(val, mp);
        // TODO pop class and args;
        stack.push(object);
        continue;
      }
      
      if (val.functionValue()->type() == FunctionType::Native) {
        // std::cout << "Native:" << std::endl;

        frames.top().pc = pc;
        frames.push(Frame(*val.functionValue())); // TODO

        std::vector<Value>& args = frames.top().locals; 
        // std::cout << "before call" << std::endl;
        stack.inspect();

        
        for (int i = 0; i < arity; i++) {
          Value& arg = stack[first + i];
          // std::cout << "arg:" << arg << std::endl;
          args.push_back(stack[first + i]);
        }
        args.resize(function->maxSlots()); // make room for the locals;
        stack.pop(arity + 1); // pop callee and args;

        // std::cout << "after pop" << std::endl;
        stack.inspect();

        RELOAD();
      } else {
        std::vector<Value> args; 
        for (int i = 0; i < arity; i++) {
          args.push_back(stack[0 - arity - i]);
        }
        stack.inspect();
        function->foreignFunction()(args);
        stack.pop(arity + 1);
      }
      continue;
    }
    case Opcode::Return: {
      // std::cout << stack.size() << std::endl;
      frames.pop(1);
      RELOAD();
      stack.inspect();
      continue;
    }

    default: {
      // std::cout << (level, tag, false) << " --------- no idea --------- " << (int)opcode << std::endl;
      // pc++;
      break;
    }
    }

  }
};

} // namespace kestrel
