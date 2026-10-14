
#include <llvm/ADT/SmallVector.h>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Error.h>

#include <memory>
#include <cstdint>

int manual_IR_generation_binary_expression(llvm::IRBuilder<>& builder);

int test_main()
{
    // 1. Initialize the native target and its assembly printer.
    if (llvm::InitializeNativeTarget()) {
        llvm::errs() << "Failed to initialize native target\n";
        return 1;
    }

    if (llvm::InitializeNativeTargetAsmPrinter()) {
        llvm::errs() << "Failed to initialize native asm printer\n";
        return 1;
    }

    // 2. Create the ORC JIT.
    auto jitExpected = llvm::orc::LLJITBuilder().create();

    if (!jitExpected) {
        llvm::logAllUnhandledErrors(
            jitExpected.takeError(),
            llvm::errs(),
            "JIT creation failed: ");
        return 1;
    }

    auto jit = std::move(*jitExpected);

    // 3. Create the LLVM context and module.
    auto context = std::make_unique<llvm::LLVMContext>();
    auto module = std::make_unique<llvm::Module>("MyLanguage", *context);

    // Use the JIT's target configuration.
    module->setDataLayout(jit->getDataLayout());
    module->setTargetTriple(jit->getTargetTriple());

    llvm::IRBuilder<> builder(*context);

    // 4. Create: i32 main()
    auto* functionType = llvm::FunctionType::get(
        builder.getInt32Ty(),
        false
    );

    auto* function = llvm::Function::Create(
        functionType,
        llvm::Function::ExternalLinkage,
        "main",
        *module
    );

    auto* entry = llvm::BasicBlock::Create(*context, "entry", function);

    builder.SetInsertPoint(entry);

    // our manual test code
    manual_IR_generation_binary_expression(builder);

    // 6. Verify the generated IR.
    if (llvm::verifyModule(*module, &llvm::errs())) {
        return 1;
    }

    module->print(llvm::outs(), nullptr);

    // 7. Transfer ownership of the module and context to ORC.
    llvm::orc::ThreadSafeModule tsm(
        std::move(module), std::move(context));

    if (auto err = jit->addIRModule(std::move(tsm))) {
        llvm::logAllUnhandledErrors(
            std::move(err), llvm::errs(),
            "Adding module failed: ");
        return 1;
    }

    // 8. Compile/resolve the function and get its native address.
    auto symbolExpected = jit->lookup("main");

    if (!symbolExpected) {
        llvm::logAllUnhandledErrors(
            symbolExpected.takeError(), llvm::errs(),
            "Function lookup failed: ");
        return 1;
    }

    // 9. Convert the JIT address to a callable function pointer.
    auto* mainFunction =
        reinterpret_cast<std::int32_t (*)()>(
            static_cast<std::uintptr_t>(
                symbolExpected->getValue()));

    // 10. Execute the generated machine code.
    std::int32_t value = mainFunction();

    llvm::outs() << "Result: " << value << "\n";

    return 0;
}

int manual_IR_generation_binary_expression(llvm::IRBuilder<>& builder)
{
        // 5. Generate: 2 * 3 - 1 + 4
    auto* one = builder.getInt32(1);
    auto* four = builder.getInt32(4);
    auto* two = builder.getInt32(2);
    auto* three = builder.getInt32(3);

    auto* product = builder.CreateMul(two, three, "product");
    auto* difference = builder.CreateSub(product, one, "difference");
    auto* result = builder.CreateAdd(four, difference, "result");

    builder.CreateRet(result);
    return 0;
}