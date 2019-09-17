#include "../include/third_party/polymorphic_value.h"
#include "../include/driver.hpp"
#include "../include/ast/ast.hpp"
#include "../include/lang/types.hpp"
#include "../include/lang/operators.hpp"
#include "../include/structs.hpp"
#include "../include/codegen/codegen.hpp"
#include <filesystem>

int main(int argc, char * argv[])
{
    bool read_from_file = false;
    Driver driver;
    Driver::set_active_instance(driver);
    
    int i;
    for (i = 1; i < argc; i++) {
        if (argv[i] == string("-p")) {
            driver.trace_parsing = true;
        } else if (argv[i] == string("-s")) {
            driver.trace_scanning = true;
        } else {
            read_from_file = true;
        }
    }

    codegen::global::fpass_manager->add(llvm::createInstructionCombiningPass());
    codegen::global::fpass_manager->add(llvm::createReassociatePass());
    codegen::global::fpass_manager->add(llvm::createNewGVNPass());
    codegen::global::fpass_manager->add(llvm::createCFGSimplificationPass());
    codegen::global::fpass_manager->add(llvm::createPromoteMemoryToRegisterPass());
    codegen::global::fpass_manager->add(llvm::createDeadInstEliminationPass());
    codegen::global::fpass_manager->add(llvm::createDeadCodeEliminationPass());
    codegen::global::fpass_manager->add(llvm::createEntryExitInstrumenterPass());
    codegen::global::fpass_manager->doInitialization();

    if (read_from_file) {
        codegen::global::module->setSourceFileName(argv[i-1]);
        driver.parse(argv[i-1]);
    } else {
        driver.parse();
    }

    std::vector<structs::Error> ast_errors = ast::AstNode::get_errors();
    std::vector<structs::Error> ast_warnings = ast::AstNode::get_warnings();

    if (!ast_errors.empty()) {
          /* Print all errors collected from the Ast */
          for (const structs::Error& err : ast_errors) {
              driver.error(err.loc, err.msg);
          }
     }

    if (!ast_warnings.empty()) {
        /* Print all warnigns collected from the Ast */
        for (const structs::Error& err : ast_warnings) {
            driver.warning(err.loc, err.msg);
        }
    }

  if (ast_errors.empty()) {
    codegen::global::fpass_manager->doFinalization();
    auto TargetTriple = llvm::sys::getDefaultTargetTriple();

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    string Error;
    auto target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

    if (!target) {
        cerr << Error;
        exit(EXIT_FAILURE);
      }


     auto CPU = "generic";
     auto features = "";
     
     llvm::TargetOptions opt;
     auto rm = llvm::Reloc::Model::PIC_;
     auto target_machine = target->createTargetMachine(TargetTriple, CPU, features, opt, rm);

     codegen::global::module->setDataLayout(target_machine->createDataLayout());
     codegen::global::module->setTargetTriple(TargetTriple);

     std::error_code EC;

     std::string fname = "output";
     if (read_from_file) {
         auto fpath = filesystem::path(argv[i-1]);
         fname = fpath.filename().replace_extension("");
     }

     llvm::raw_fd_ostream dest(fname + ".o", EC, llvm::sys::fs::F_None);
     
     if (EC) {
           cerr << "Could not open file: " << EC.message();
           exit(EXIT_FAILURE);
     }

     llvm::legacy::PassManager pass;
     auto FileType = llvm::TargetMachine::CGFT_ObjectFile;
     
     if (target_machine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
       cerr << "TargetMachine can't emit a file of this type";
       return 1;
     }
     
        pass.run(*codegen::global::module);
        dest.flush();
 
       llvm::raw_fd_ostream ll_out(fname + ".ll", EC, llvm::sys::fs::F_None);
       if (EC) {
           cerr << "Could not open file: " << EC.message();
           exit(EXIT_FAILURE);
     }

       codegen::global::module->print(ll_out, nullptr);
       ll_out.flush();
       ll_out.close();
    }

	return 0;
}
