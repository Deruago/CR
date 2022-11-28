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
    template<typename... Ts>
    struct stuc
    {
        template<const str&... name>
        struct names
        {
            template<typename... InheritTs>
            struct inherit
            {
                struct ImplementationCore {
                    template<int count, typename OriginalT, typename DataT>
                    struct DataLeaf
                    {
                        using original_type = OriginalT;
                        using type = DataT;
                        DataT value;
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

                    template<typename derived>
                    struct InheritScope : InheritImpl<0, derived, InheritTs...>
                    {
                        template<int localId, typename next>
                        auto& get_alt()
                        {
                            return this->next::template get_alt<localId>();
                        }
                    };
                    
                    template<typename derived>
                    struct DataScope : DataImpl<0, derived, Ts...>
                    {
                    };

                    struct Implementation : 
                        private InheritScope<Implementation>,
                        DataScope<Implementation>
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

                            static constexpr auto total_members = sizeof...(Ts) + GetTotalMembers<InheritTs...>::value;

                            static constexpr auto total_direct_inherited_bases = sizeof...(InheritTs);

                            template<int baseId>
                            struct GetBaseByBaseId
                            {
                                template<int baseId_, typename... InheritTs_>
                                struct GetBaseByBaseIdImpl
                                {

                                };

                                template<int count, typename T_, typename... Ts_>
                                struct GetBaseByBaseIdImpl<count, T_, Ts_...>
                                {
                                    using type = typename GetBaseByBaseIdImpl<count - 1, Ts_...>::type;
                                };

                                template<typename T_, typename... Ts_>
                                struct GetBaseByBaseIdImpl<0, T_, Ts_...>
                                {
                                    using type = T_;
                                };
                                using type = typename GetBaseByBaseIdImpl<baseId, InheritTs...>::type;
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
                                    static constexpr auto value = count - T_::meta::total_members;
                                    using type = this_type;
                                };

                                template<int count>
                                struct InheritType_FromGeneralIdImpl<count, false>
                                {
                                    static constexpr auto value = count;
                                    using type = this_type;
                                };

                                template<int count, typename T_, typename... Ts_>
                                struct InheritType_FromGeneralIdImpl<count, false, T_, Ts_...>
                                {
                                    static constexpr auto new_count = count - T_::meta::total_members;
                                    using type = typename InheritType_FromGeneralIdImplStart<new_count, Ts_...>::type;
                                };

                                template<int count, typename T_, typename... Ts_>
                                struct InheritType_FromGeneralIdImpl<count, true, T_, Ts_...>
                                {
                                    static constexpr auto value = count;
                                    using type = T_;
                                };

                                template<int count, typename... Ts_>
                                struct InheritType_FromGeneralIdImplStart
                                {
                                    static constexpr auto value = count;
                                    using type = this_type;
                                };

                                template<int count, typename T_, typename... Ts_>
                                struct InheritType_FromGeneralIdImplStart<count, T_, Ts_...>
                                {
                                    static constexpr auto value = count;
                                    using type = typename InheritType_FromGeneralIdImpl<
                                        count,
                                        (count < T_::meta::total_members),
                                        T_,
                                        Ts_...>
                                    ::type;
                                };

                                using type = typename InheritType_FromGeneralIdImplStart<memberId, InheritTs...>::type;
                                static constexpr auto value = InheritType_FromGeneralIdImplStart<memberId, InheritTs...>::value;
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
                                typename meta::template InheritType_FromGeneralId<id>::type;
                            static constexpr auto localId =
                                meta::template LocalId_FromGeneralId<id>::value;

                            using dataLeafLowered = typename correctStruct::template DataLeaf<localId, correctStruct, getT>;

                            static_assert(std::is_base_of<
                                dataLeafLowered,
                                Implementation>::value, 
                                "Given Id is not covered by any of the bases or internally.");

                            return this->dataLeafLowered::value;
                        }

                        template<int id>
                        auto& get()
                        {
                            using correctStruct = 
                                typename meta::template InheritType_FromGeneralId<id>::type;
                            static constexpr auto localId =
                                meta::template LocalId_FromGeneralId<id>::value;
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
                                return correctStruct::template get<localId>();
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

                        template<int id>
                        auto& get_alt()
                        {
                            using correctStruct = 
                                typename meta::template InheritType_FromGeneralId<id>::type;
                            static constexpr auto localId =
                                meta::template InheritType_FromGeneralId<id>::value;
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
                                return  InheritScope<this_type>::template get_alt<localId, correctStruct>();
                            }
                        }
                    };
                    using type = Implementation;
                };

                using type = typename ImplementationCore::Implementation;
            };

            using type = typename inherit<>::type;
        };

        using type = typename names<>::type;
    };
}

#endif // CR_CORE_META_FUNCTION_HEADER_CR_H