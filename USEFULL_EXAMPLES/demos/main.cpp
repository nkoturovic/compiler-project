
    // Symbol Table demo

    codegen::SymbolTable st;
    st.begin_scope();
    st.define_variable("a", {poly_type(lang::types::IntType()), nullptr});
    std::optional<structs::TypeValuePair> opt_a = st.get_variable("a");
    if (opt_a.has_value())
        std::cout << opt_a->type->str() << std::endl;
    else
        std::cout << "Error finding value" << std::endl;

    st.push_scope();

    std::optional<structs::TypeValuePair> opt1_a = st.get_variable("a");
    if (opt1_a.has_value())
        std::cout << opt1_a->type->str() << std::endl;
    else
        std::cout << "Error finding value" << std::endl;

    st.pop_scope();

    st.begin_scope();
    st.define_variable("b", {poly_type(lang::types::DoubleType()), nullptr});

    std::optional<structs::TypeValuePair> opt2_a = st.get_variable("a");
    if (opt2_a.has_value())
        std::cout << opt2_a->type->str() << std::endl;
    else
        std::cout << "Error finding value" << std::endl;

    std::optional<structs::TypeValuePair> opt_b = st.get_variable("b");

    if (opt_b.has_value())
        std::cout << opt_b->type->str() << std::endl;
    else
        std::cout << "Error finding value" << std::endl;

    st.end_scope();

    std::optional<structs::TypeValuePair> opt1_b = st.get_variable("b");
    if (opt1_b.has_value())
        std::cout << opt1_b->type->str() << std::endl;
    else
        std::cout << "Error finding value" << std::endl;

    st.end_scope();

    std::optional<structs::TypeValuePair> opt3_a = st.get_variable("a");
    if (opt3_a.has_value())
        std::cout << opt3_a->type->str() << std::endl;
    else
        std::cout << "Error finding value" << std::endl;


