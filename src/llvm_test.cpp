#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

int test_main()
{
    // 1. Own the LLVM data structures.
    llvm::LLVMContext context;
    llvm::Module module("MyLanguage", context);
    llvm::IRBuilder<> builder(context);
    
    // 2. Create a function:
    //    i32 main()
    auto* functionType = llvm::FunctionType::get(
        builder.getInt32Ty(),
        false
    );

    auto* function = llvm::Function::Create(
        functionType,
        llvm::Function::ExternalLinkage,
        "main",
        module
    );

    // 3. Create the entry basic block.
    auto* entry = llvm::BasicBlock::Create(
        context,
        "entry",
        function
    );

    builder.SetInsertPoint(entry);

    // 4. Generate the expression 1 + 4 - 2 * 3.
    auto* two = builder.getInt32(2);
    auto* three = builder.getInt32(3);
    auto* one = builder.getInt32(1);
    auto* four = builder.getInt32(4);

    auto* product = builder.CreateMul(two, three, "product");
    auto* product2 = builder.CreateSub(product, four, "product");
    auto* result = builder.CreateAdd(one, product2, "result");

    // 5. Return the result.
    builder.CreateRet(result);

    // 6. Check that the generated IR is valid.
    if (llvm::verifyModule(module, &llvm::errs())) {
        return 1;
    }

    // 7. Print the LLVM IR.
    module.print(llvm::outs(), nullptr);

    return 0;
}