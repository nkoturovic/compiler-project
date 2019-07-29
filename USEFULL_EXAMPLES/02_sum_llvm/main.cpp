#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Instruction.h>

int main()
{ 
    /* Context - Sadrzi gomulu llvm deklaracija
     * f-ja API-ja zahtevaju context */
    llvm::LLVMContext context;

    /* Objekat pomocu koga generisemo llvm IR instrukcije 
     * NoFolder template arg kaze builderu da ne radi Expr folding */
    llvm::IRBuilder<llvm::NoFolder> builder(context);
   
    /* Pri generaciji koda, sav nas kod bude spakovan u modul */
    llvm::Module * module = new llvm::Module("ime_modula", context);

    /* Pravimo deklaraciju main funkcije */
    llvm::FunctionType *funcType =
      llvm::FunctionType::get(builder.getInt32Ty(), false);
    llvm::Function *mainFunc =
      llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);

    /* Definicija int main() > void(i32?) @main() { entrypoint: } */
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
    builder.SetInsertPoint(entry);

    /* Generisacemo jednostavan kod za C-ovski izraz: 5 + 3
     * Value je bazna klasa klase ConstantInt, kao i ostalih klasa medjukoda (IR-a) */
    llvm::ConstantInt * lhs = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 5);
    llvm::ConstantInt * rhs = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 3);

	/* Value je bazicni blok i sve instrukcije i vrednosti koje se mogu javiti u llvm IR nasledjuju 
	* Ovde Value cuva odgovarajuci medjukod (IR) koji odgovara instrukciji za sabiranje dva Int-a */
	llvm::Value * result = builder.CreateAdd(lhs, rhs, "result_tmp");

    builder.CreateRet(result); // Vracamo rezultat izracunavanja u consolu

    /* Stampamo sve sto je generisano u nasem modulu */
    module->print(llvm::outs(), nullptr);

    delete module; // oslobadjanje memorije

    return 0;
}
