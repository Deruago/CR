/* Made By: Thimo Böhmer
 * Date: November 2022
 */

#ifndef CR_CORE_META_FUNCTION_HEADER_CR_H
#define CR_CORE_META_FUNCTION_HEADER_CR_H

#include <type_traits>

namespace cr
{
    struct str { const char *text; };
}

namespace cr::meta
{
    template<typename... Ts>
    struct VariadicTypenameCache
    {
        
    };

    template<template<typename...> typename target, typename... finiteArgs>
    struct extract_variadic
    {
        template<typename... variadic>
        static auto forward(::cr::meta::VariadicTypenameCache<variadic...>)
        {
            using new_forwarded_type = target<finiteArgs..., variadic...>;
            return std::declval<new_forwarded_type>();
        }

        template<typename variadic>
        using forward_t = decltype(forward(std::declval<variadic>()));
    };

    template<typename... Ts>
    struct FirstOfVariadic
    {
        using type = void;
    };

    template<typename T, typename... Ts>
    struct FirstOfVariadic<T, Ts...>
    {
        using type = T;
    };

    template<int count, typename... Ts>
    struct NthOfVariadic
    {
        using type = void;
    };

    template<int count, typename T, typename... Ts>
    struct NthOfVariadic<count, T, Ts...>
    {
        using type = typename  NthOfVariadic<count - 1, Ts...>::type;
    };

    template<typename T, typename... Ts>
    struct NthOfVariadic<0, T, Ts...>
    {
        using type = T;
    };
}

namespace cr::gen
{
    template<typename returnType_, typename... statements>
    struct logic
    {
        using returnType = returnType_;
        using stmts =  typename ::cr::meta::VariadicTypenameCache<statements...>;
    };

    struct create_variable_base {};
    template<typename T, typename Q>
    struct create_variable : create_variable_base
    {
        using type = T;
        using initialize_stmt = Q;
    };

    struct return_variable_base {};
    template<typename T>
    struct return_variable : return_variable_base
    {
        using return_stmt = T;
    };

    struct create_int_base {};
    template<int val_>
    struct create_int : create_int_base
    {
        static constexpr int value = val_;
    };

    struct create_bool_base {};
    template<bool val_>
    struct create_bool : create_bool_base
    {
        static constexpr bool value = val_;
    };

    // Variable reference, where id == variable id
    struct var_base {};
    template<int id>
    struct var : var_base
    {
        static constexpr int value = id;
    };

    struct add_base {};
    template<typename LHS_, typename RHS_>
    struct add : add_base
    {
        using lhs = LHS_;
        using rhs = RHS_;
    };

    struct if_base {};
    template<typename conditional_stmt_, typename action_stmt_>
    struct iff : if_base
    {
        using conditional_stmt = conditional_stmt_;
        using action_stmt = action_stmt_;
    };

    struct elif_base {};
    template<typename conditional_stmt_, typename action_stmt_>
    struct elif : elif_base
    {
        using conditional_stmt = conditional_stmt_;
        using action_stmt = action_stmt_;
    };

    struct else_base {};
    template<typename action_stmt_>
    struct els : else_base
    {
        using action_stmt = action_stmt_;
    };

    struct forc_base {};
    template<typename for_count_stmt_>
    struct forc : forc_base
    {
        using for_count_stmt = for_count_stmt_;
    };
}

namespace cr::gen
{
    namespace interpreter
    {
        template<typename originalDatastructure, typename memberFunction, typename... stmts>
        struct parse_stmts
        {
            template<int user_argument_count, typename... Arguments>
            static inline typename memberFunction::returnType function_impl(Arguments... args [[maybe_unused]])
            {
                return typename memberFunction::returnType{};
            }
        };

        template<typename originalDatastructure, typename memberFunction, typename stmt, typename... stmts>
        struct parse_stmts<originalDatastructure, memberFunction, stmt, stmts...>
        {
            template<int user_argument_count, typename... Arguments>
            static inline typename memberFunction::returnType function_start(Arguments&&... args [[maybe_unused]])
            {
                return function_impl<user_argument_count>(args...);
            }

            template<int user_argument_count, typename... Arguments>
            static inline auto function_impl(Arguments&&... args [[maybe_unused]])
            {
                // Dispatcher for stmt resolvement
                // Uses a concrete base check, to validate the type of stmt
                if constexpr (std::is_base_of_v<create_variable_base, stmt>)
                {
                    return ::cr::gen::interpreter
                        ::parse_stmts<originalDatastructure, memberFunction, stmt, stmts...>
                        ::template function_create_variable<user_argument_count>(args...);
                }
                else if constexpr(std::is_base_of_v<create_int_base, stmt>)
                {
                    return ::cr::gen::interpreter
                        ::parse_stmts<originalDatastructure, memberFunction, stmt, stmts...>
                        ::template function_create_int<user_argument_count>(args...);
                }
                else if constexpr(std::is_base_of_v<create_bool_base, stmt>)
                {
                    return ::cr::gen::interpreter
                        ::parse_stmts<originalDatastructure, memberFunction, stmt, stmts...>
                        ::template function_create_bool<user_argument_count>(args...);
                }
                else if constexpr(std::is_base_of_v<return_variable_base, stmt>)
                {
                    return ::cr::gen::interpreter
                        ::parse_stmts<originalDatastructure, memberFunction, stmt, stmts...>
                        ::template function_return_variable<user_argument_count>(args...);
                }
                else if constexpr(std::is_base_of_v<var_base, stmt>)
                {
                    return ::cr::gen::interpreter
                        ::parse_stmts<originalDatastructure, memberFunction, stmt, stmts...>
                        ::template function_var<user_argument_count>(args...);
                }
                else if constexpr(std::is_base_of_v<add_base, stmt>)
                {
                    return ::cr::gen::interpreter
                        ::parse_stmts<originalDatastructure, memberFunction, stmt, stmts...>
                        ::template function_add<user_argument_count>(args...);
                }
                else if constexpr(std::is_base_of_v<if_base, stmt>)
                {

                }
                else if constexpr(std::is_base_of_v<elif_base, stmt>)
                {

                }
                else if constexpr(std::is_base_of_v<else_base, stmt>)
                {

                }
                else if constexpr(std::is_base_of_v<forc_base, stmt>)
                {

                }
                else
                {
                    return typename memberFunction::returnType{};
                }
            }

            template<int user_argument_count, typename... Arguments>
            static inline auto function_create_variable(Arguments&&... args [[maybe_unused]])
            {
                typename stmt::type new_tmp_variable = ::cr::gen::interpreter
                    ::parse_stmts<originalDatastructure, memberFunction, typename stmt::initialize_stmt>
                    ::template function_impl<user_argument_count>(args...);
                
                return ::cr::gen::interpreter
                    ::parse_stmts<originalDatastructure, memberFunction, stmts...>
                    ::template function_impl<user_argument_count, Arguments..., typename stmt::type&>(args..., new_tmp_variable);
            }

            template<int count, typename Argument, typename... Arguments>
            static inline auto get_argument(Argument&& arg [[maybe_unused]], Arguments&&... args [[maybe_unused]])
            {
                if constexpr(count <= 0)
                {
                    return arg;
                }
                else
                {
                    if constexpr (sizeof...(Arguments) == 0)
                    {
                        // Error, not possible to expand
                        return;
                    }
                    else
                    {
                        return get_argument<count - 1>(args...);
                    }
                }
            }

            template<int user_argument_count, typename... Arguments>
            static inline auto function_var(Arguments&&... args [[maybe_unused]])
            {
                if constexpr (sizeof...(Arguments) == 0)
                {
                    // Error, nothing to retrieve
                    return;
                }
                else if constexpr (stmt::value >= sizeof...(Arguments))
                {
                    // Tried to access argument that does not exist
                    return;
                }
                else
                {
                    return get_argument<stmt::value + 1>(args...);
                }
            }

            template<int user_argument_count, typename... Arguments>
            static inline int function_create_int(Arguments&&... args [[maybe_unused]])
            {
                return stmt::value;
            }

            template<int user_argument_count, typename... Arguments>
            static inline bool function_create_bool(Arguments&&... args [[maybe_unused]])
            {
                return stmt::value;
            }

            template<int user_argument_count, typename... Arguments>
            static inline auto function_add(Arguments&&... args [[maybe_unused]])
            {
                return ::cr::gen::interpreter
                    ::parse_stmts<originalDatastructure, memberFunction, typename stmt::lhs>
                    ::template function_impl<user_argument_count, Arguments...>(args...)
                    +
                    ::cr::gen::interpreter
                    ::parse_stmts<originalDatastructure, memberFunction, typename stmt::rhs>
                    ::template function_impl<user_argument_count, Arguments...>(args...);
            }

            template<int user_argument_count, typename... Arguments>
            static inline typename memberFunction::returnType function_return_variable(Arguments&&... args [[maybe_unused]])
            {                
                return ::cr::gen::interpreter
                    ::parse_stmts<originalDatastructure, memberFunction, typename stmt::return_stmt>
                    ::template function_impl<user_argument_count>(args...);
            }
        };
    }

    template<typename originalDatastructure, typename memberFunction>
    struct member_function
    {
        struct lower_retrieve_stmts
        {
            template<typename... stmts>
            static auto retrieve_stmt(::cr::meta::VariadicTypenameCache<stmts...>)
            {
                return interpreter::parse_stmts<originalDatastructure, memberFunction, stmts...>{};
                // return std::declval<interpreter
                //     ::parse_stmts<originalDatastructure, memberFunction, stmts...>>();
            }
        };

        template<typename... UserArguments>
        inline typename memberFunction::returnType function_start(UserArguments&&... args)
        {
            return function_impl<sizeof...(UserArguments), UserArguments...>(args...);
        }

        template<int user_argument_count, typename... Arguments>
        inline typename memberFunction::returnType function_impl(Arguments&&... args [[maybe_unused]])
        {
            // using parse_stmts = typename ::cr::meta
            //         ::extract_variadic<interpreter::parse_stmts, originalDatastructure, memberFunction>
            //         ::template forward_t<typename memberFunction::stmts>;

            using parse_stmts = decltype(
                lower_retrieve_stmts::retrieve_stmt(
                    typename memberFunction::stmts{}
                )
            );
            return parse_stmts::template function_start<user_argument_count, Arguments...>(this, args...);
        }
    };
}

namespace cr::gen
{
    template<typename... Ts>
    struct stuc
    {
        template<const str&... name>
        struct names
        {
            template<typename... InheritTs>
            struct inherit
            {
                template<typename... MemberFunctionTs>
                struct memberfunction
                {
                    struct ImplementationCore {
                        template<int count, typename OriginalT, typename DataT>
                        struct DataLeaf
                        {
                            using original_type = OriginalT;
                            using type = DataT;
                            std::conditional_t<std::is_same_v<DataT, void>, int, DataT> value;
                        };

                        template<int count, typename OriginalT, typename... DataTs>
                        struct DataImpl
                        {
                        };

                        template<int count, typename OriginalT, typename DataT, typename... DataTs>
                        struct DataImpl<count, OriginalT, DataT, DataTs...> : DataLeaf<count, OriginalT, DataT>, DataImpl<count + 1, OriginalT, DataTs...>
                        {
                        };

                        template<int count, typename OriginalT>
                        struct DataImpl<count, OriginalT>
                        {
                        };

                        template<int count, typename OriginalT, typename InheritT>
                        struct InheritLeaf : InheritT
                        {
                            using inherited_type = InheritT;
                        };
                        
                        template<int count, typename OriginalT, typename... InheritTs_>
                        struct InheritImpl
                        {
                        };

                        template<int count, typename OriginalT, typename InheritT, typename... InheritTs_>
                        struct InheritImpl<count, OriginalT, InheritT, InheritTs_...> : InheritLeaf<count, OriginalT, InheritT>, InheritImpl<count + 1, OriginalT, InheritTs_...>
                        {
                        };

                        template<int count, typename OriginalT>
                        struct InheritImpl<count, OriginalT>
                        {
                        };

                        template<int count, typename OriginalT, typename MemberFunctionT>
                        struct MemberFunctionLeaf : ::cr::gen::member_function<OriginalT, MemberFunctionT>
                        {
                            using member_function_type = MemberFunctionT;
                        };
                        
                        template<int count, typename OriginalT, typename... MemberFunctionTs_>
                        struct MemberFunctionImpl
                        {
                        };

                        template<int count, typename OriginalT, typename MemberFunctionT, typename... MemberFunctionTs_>
                        struct MemberFunctionImpl<count, OriginalT, MemberFunctionT, MemberFunctionTs_...> : MemberFunctionLeaf<count, OriginalT, MemberFunctionT>, MemberFunctionImpl<count + 1, OriginalT, MemberFunctionTs_...>
                        {
                        };

                        template<int count, typename OriginalT>
                        struct MemberFunctionImpl<count, OriginalT>
                        {
                        };

                        template<typename derived>
                        struct InheritScope : InheritImpl<0, derived, InheritTs...>
                        {
                        };
                        
                        template<typename derived>
                        struct DataScope : DataImpl<0, derived, Ts...>
                        {
                        };

                        template<typename derived>
                        struct MemberFunctionScope : MemberFunctionImpl<0, derived, MemberFunctionTs...>
                        {
                        };

                        struct Implementation : 
                            InheritScope<Implementation>,
                            DataScope<Implementation>,
                            MemberFunctionScope<Implementation>
                        {
                            using this_type = Implementation;

                            struct meta
                            {
                                // Owning members are members in this data structure
                                // Excluding base members that are inherited.
                                static constexpr auto total_owning_members = sizeof...(Ts);

                                // Including base members
                                template<typename... ts_>
                                struct GetTotalMembers
                                {
                                    static constexpr auto value = 0;
                                };

                                template<typename t_, typename... ts_>
                                struct GetTotalMembers<t_, ts_...>
                                {
                                    static constexpr auto value = t_::meta::total_members + GetTotalMembers<ts_...>::value;
                                };

                                static constexpr auto total_members = total_owning_members + GetTotalMembers<InheritTs...>::value;

                                static constexpr auto total_direct_inherited_bases = sizeof...(InheritTs);

                                template<int baseId>
                                struct GetBaseByBaseId
                                {
                                    using type = typename ::cr::meta::template NthOfVariadic<baseId, InheritTs...>::type;
                                };

                                template<int memberId>
                                struct DirectClassId_FromMemberId
                                {
                                    template<int count, typename... Ts_>
                                    struct LocalId_FromGeneralIdImplStart;

                                    template<int baseId_, bool covered, typename... InheritTs_>
                                    struct LocalId_FromGeneralIdImpl
                                    {
                                    };

                                    template<int count, typename T_>
                                    struct LocalId_FromGeneralIdImpl<count, false, T_>
                                    {
                                        using type = this_type;
                                    };

                                    template<int count>
                                    struct LocalId_FromGeneralIdImpl<count, false>
                                    {
                                        using type = this_type;
                                    };

                                    template<int count, bool covered, typename T_, typename... Ts_>
                                    struct LocalId_FromGeneralIdImpl<count, covered, T_, Ts_...>
                                    {
                                        static constexpr auto new_count = count - T_::meta::total_members;
                                        using type = typename LocalId_FromGeneralIdImplStart<new_count, Ts_...>::type;
                                    };

                                    template<int count, typename T_, typename... Ts_>
                                    struct LocalId_FromGeneralIdImpl<count, true, T_, Ts_...>
                                    {
                                        using type = T_;
                                    };

                                    template<int count, typename... Ts_>
                                    struct LocalId_FromGeneralIdImplStart
                                    {
                                        using type = this_type;
                                    };

                                    template<int count, typename T_, typename... Ts_>
                                    struct LocalId_FromGeneralIdImplStart<count, T_, Ts_...>
                                    {
                                    using type = typename LocalId_FromGeneralIdImpl<
                                            count,
                                            (count < T_::meta::total_members),
                                            T_,
                                            Ts_...>
                                        ::type;
                                    };

                                    using type = typename LocalId_FromGeneralIdImplStart<memberId, InheritTs...>::type;
                                };

                                template<int memberId>
                                struct LocalId_FromGeneralId
                                {
                                    template<int count, typename... Ts_>
                                    struct LocalId_FromGeneralIdImplStart;

                                    template<int baseId_, bool covered, typename... InheritTs_>
                                    struct LocalId_FromGeneralIdImpl
                                    {
                                    };

                                    template<int count, typename T_>
                                    struct LocalId_FromGeneralIdImpl<count, false, T_>
                                    {
                                        static constexpr auto value = count - T_::meta::total_members;
                                    };

                                    template<int count>
                                    struct LocalId_FromGeneralIdImpl<count, false>
                                    {
                                        static constexpr auto value = count;
                                    };

                                    template<int count, bool covered, typename T_, typename... Ts_>
                                    struct LocalId_FromGeneralIdImpl<count, covered, T_, Ts_...>
                                    {
                                        static constexpr auto new_count = count - T_::meta::total_members;
                                        static constexpr auto value = LocalId_FromGeneralIdImplStart<new_count, Ts_...>::value;
                                    };

                                    template<int count, typename T_, typename... Ts_>
                                    struct LocalId_FromGeneralIdImpl<count, true, T_, Ts_...>
                                    {
                                        static constexpr auto value = T_::meta::template LocalId_FromGeneralId<count>::value;
                                    };

                                    template<int count, typename... Ts_>
                                    struct LocalId_FromGeneralIdImplStart
                                    {
                                        static constexpr auto value = count;
                                    };

                                    template<int count, typename T_, typename... Ts_>
                                    struct LocalId_FromGeneralIdImplStart<count, T_, Ts_...>
                                    {
                                        static constexpr auto value = LocalId_FromGeneralIdImpl<
                                            count,
                                            (count < T_::meta::total_members),
                                            T_,
                                            Ts_...>
                                        ::value;
                                    };

                                    static constexpr auto value = LocalId_FromGeneralIdImplStart<memberId, InheritTs...>::value;
                                };

                                template<int memberId>
                                struct InheritType_FromGeneralId
                                {
                                    template<int count, typename... Ts_>
                                    struct InheritType_FromGeneralIdImplStart;

                                    template<int baseId_, bool covered, typename... InheritTs_>
                                    struct InheritType_FromGeneralIdImpl
                                    {
                                    };

                                    template<int count, typename T_>
                                    struct InheritType_FromGeneralIdImpl<count, false, T_>
                                    {
                                        static constexpr int value = count - T_::meta::total_members;
                                        using type = this_type;
                                    };

                                    template<int count>
                                    struct InheritType_FromGeneralIdImpl<count, false>
                                    {
                                        static constexpr int value = count;
                                        using type = this_type;
                                    };

                                    template<int count, typename T_, typename... Ts_>
                                    struct InheritType_FromGeneralIdImpl<count, false, T_, Ts_...>
                                    {
                                        static constexpr int new_count = count - T_::meta::total_members;
                                        using type = typename InheritType_FromGeneralIdImplStart<new_count, Ts_...>::type;
                                    };

                                    template<int count, typename T_, typename... Ts_>
                                    struct InheritType_FromGeneralIdImpl<count, true, T_, Ts_...>
                                    {
                                        static constexpr int value = count;
                                        using type = T_;
                                    };

                                    template<int count, typename... Ts_>
                                    struct InheritType_FromGeneralIdImplStart
                                    {
                                        static constexpr int value = count;
                                        using type = this_type;
                                    };

                                    template<int count, typename T_, typename... Ts_>
                                    struct InheritType_FromGeneralIdImplStart<count, T_, Ts_...>
                                    {
                                        static constexpr int value = count;
                                        using type = typename InheritType_FromGeneralIdImpl<
                                            count,
                                            (count < T_::meta::total_members),
                                            T_,
                                            Ts_...>
                                        ::type;
                                    };

                                    using type = typename InheritType_FromGeneralIdImplStart<memberId, InheritTs..., this_type>::type;
                                    static constexpr auto value = InheritType_FromGeneralIdImplStart<memberId, InheritTs..., this_type>::value;
                                };

                                template<int memberId>
                                struct InheritBaseId_FromGeneralId
                                {
                                    template<int base_id_, int count, typename... ts_>
                                    struct Start;

                                    template<int base_id_, int count, bool inRange, typename... ts_>
                                    struct Check
                                    {
                                        // Maps to void if the index was not mapped
                                        // Otherwise maps to this_type
                                        using type = typename std::conditional<
                                            (count < this_type::meta::total_owning_members),
                                            this_type,
                                            void>::type;
                                        static constexpr auto value = count;
                                        static constexpr auto base_id = base_id_;
                                    };

                                    template<int base_id_, int count, typename t_, typename... ts_>
                                    struct Check<base_id_, count, true, t_, ts_...>
                                    {
                                        using type = t_;
                                        static constexpr auto value = count;
                                        static constexpr auto base_id = base_id_;
                                    };

                                    template<int base_id_, int count, typename t_, typename... ts_>
                                    struct Check<base_id_, count, false, t_, ts_...>
                                    {
                                        static constexpr int new_count = count - t_::meta::total_members;
                                        using type = typename Start<base_id_ + 1, new_count, ts_...>::type;
                                            
                                        static constexpr int value = Start<base_id_ + 1, new_count, ts_...>::value;
                                        static constexpr int base_id = Start<base_id_ + 1, new_count, ts_...>::base_id;
                                    };

                                    template<int base_id_, int count, typename... ts_>
                                    struct Start
                                    {
                                        using type = typename Check<
                                            base_id_,
                                            count,
                                            false>::type;
                                        static constexpr int value = Check<
                                            base_id_,
                                            count,
                                            false>::value;
                                        static constexpr int base_id = Check<
                                            base_id_,
                                            count,
                                            false>::base_id;
                                    };

                                    template<int base_id_, int count, typename t_, typename... ts_>
                                    struct Start<base_id_, count, t_, ts_...>
                                    {
                                        using type = typename Check<
                                            base_id_,
                                            count,
                                            (count < t_::meta::total_members),
                                            t_,
                                            ts_...>::type;
                                        static constexpr int value = Check<
                                            base_id_,
                                            count,
                                            (count < t_::meta::total_members),
                                            t_,
                                            ts_...>::value;
                                        static constexpr int base_id = Check<
                                            base_id_,
                                            count,
                                            (count < t_::meta::total_members),
                                            t_,
                                            ts_...>::base_id;
                                    };

                                    using type = typename Start<0, memberId, InheritTs...>::type;
                                    static constexpr int value = Start<0, memberId, InheritTs...>::value;
                                    static constexpr int base_id = Start<0, memberId, InheritTs...>::base_id;
                                };

                                template<int id>
                                struct GetTypeFromId
                                {
                                    template<int id_, typename... Ts_>
                                    struct GetTypeFromIdImpl
                                    {
                                        using type = void;
                                    };

                                    template<int count, typename T_, typename... Ts_>
                                    struct GetTypeFromIdImpl<count, T_, Ts_...>
                                    {
                                        using type = typename GetTypeFromIdImpl<count - 1, Ts_...>::type;
                                    };

                                    template<typename T_, typename... Ts_>
                                    struct GetTypeFromIdImpl<0, T_, Ts_...>
                                    {
                                        using type = T_;
                                    };
                                    using type = typename GetTypeFromIdImpl<id, Ts...>::type;
                                };

                                template<const str& idName>
                                struct GetIdFromName
                                {
                                    template<int count, const str& requestIdName, const str&... idNames>
                                    struct GetIdFromNameImpl
                                    {
                                        static constexpr int value = count;
                                    };

                                    template<int count, const str& requestIdName, const str& currentIdName, const str&... idNames>
                                    struct GetIdFromNameImpl<count, requestIdName, currentIdName, idNames...>
                                    {
                                        static constexpr int value = GetIdFromNameImpl<count + 1, requestIdName, idNames...>::value;
                                    };

                                    template<int count, const str& requestIdName, const str&... idNames>
                                    struct GetIdFromNameImpl<count, requestIdName, requestIdName, idNames...>
                                    {
                                        static constexpr int value = count;
                                    };
                                static constexpr int value = GetIdFromNameImpl<0, idName, name...>::value;
                                };
                            };

                            template<int id, typename getT>
                            auto& get()
                            {
                                using correctStruct = 
                                    typename meta::template InheritBaseId_FromGeneralId<id>::type;
                                
                                static constexpr auto localId =
                                    meta::template InheritBaseId_FromGeneralId<id>::value;
                                static constexpr auto baseId = 
                                    meta::template InheritBaseId_FromGeneralId<id>::base_id;

                                if constexpr (std::is_same_v<this_type, correctStruct>)
                                {
                                    using getTCompare = typename correctStruct::meta::template GetTypeFromId<localId>::type;

                                    static_assert(std::is_same_v<getT, getTCompare>, "Given T is not the Expected Type");

                                    using dataLeafLowered = typename DataScope<this_type>::template DataLeaf<localId, correctStruct, getT>;

                                    static_assert(std::is_base_of<
                                        dataLeafLowered,
                                        Implementation>::value, 
                                        "Given Id is not covered by any of the bases or internally.");

                                    return this->dataLeafLowered::value;
                                }
                                else
                                {
                                    using requiredInheritLeaf = typename InheritScope<this_type>::template InheritLeaf<baseId, this_type, correctStruct>;
                                    return requiredInheritLeaf::template get<localId>();
                                }
                            }

                            template<int id>
                            auto& get()
                            {
                                using correctStruct = 
                                    typename meta::template InheritBaseId_FromGeneralId<id>::type;
                                
                                static constexpr auto localId =
                                    meta::template InheritBaseId_FromGeneralId<id>::value;
                                static constexpr auto baseId = 
                                    meta::template InheritBaseId_FromGeneralId<id>::base_id;

                                if constexpr (std::is_same_v<this_type, correctStruct>)
                                {
                                    using getT = typename correctStruct::meta::template GetTypeFromId<localId>::type;

                                    using dataLeafLowered = typename DataScope<this_type>::template DataLeaf<localId, correctStruct, getT>;

                                    static_assert(std::is_base_of<
                                        dataLeafLowered,
                                        Implementation>::value, 
                                        "Given Id is not covered by any of the bases or internally.");

                                    return this->dataLeafLowered::value;
                                }
                                else
                                {
                                    using requiredInheritLeaf = typename InheritScope<this_type>::template InheritLeaf<baseId, this_type, correctStruct>;
                                    return requiredInheritLeaf::template get<localId>();
                                }
                            }
                            template<const str& idName>
                            auto& get()
                            {
                                static constexpr auto id = meta::template GetIdFromName<idName>::value;

                                return this->DataLeaf<
                                    id,
                                    Implementation,
                                    typename meta::template GetTypeFromId<id>::type
                                >::value;
                            }
                        };
                        using type = Implementation;
                    };
    
                    using type = typename ImplementationCore::Implementation;
                };

                template<typename... MemberFunctionTs>
                using memfun = typename memberfunction<MemberFunctionTs...>::type;

                using type = typename memberfunction<>::type;
            };

            using type = typename inherit<>::type;
        };

        template<typename... InheritTs>
        struct inherit
        {
            using type = typename ::cr::gen::stuc<Ts...>::names<>::template inherit<InheritTs...>::type;
        };

        using type = typename names<>::type;
    };

    template<typename... InheritTs>
    struct inh
    {
        using type = typename ::cr::gen::stuc<>::names<>::inherit<InheritTs...>::type;
    };
}

#endif // CR_CORE_META_FUNCTION_HEADER_CR_H