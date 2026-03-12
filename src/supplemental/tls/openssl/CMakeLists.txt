include(FindThreads)

if (NNG_TLS_ENGINE STREQUAL "open")
    message(NOTICE "
        ************************************************************
        Linking against OpenSSL may change license terms.
        Consult a lawyer and the license files for details.
        ************************************************************")
    nng_sources(openssl.c)

    # If OpenSSL was added by a consuming project, then we should use that
    # instance of it, instead of configuring our own.
    if (TARGET openssl)
        nng_link_libraries(openssl)
    else()
        # We want to prefer config mode over our local find package.
        if (NOT (DEFINED CMAKE_FIND_PACKAGE_PREFER_CONFIG))
            set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
            find_package(OpenSSL REQUIRED)
            unset(CMAKE_FIND_PACKAGE_PREFER_CONFIG)
        else()
            find_package(OpenSSL REQUIRED)
        endif()
        if (NNG_REQUIRE_PKCS11_PROVIDER)
            if (DEFINED OpenSSL_VERSION)
                set(NNG_OPENSSL_VERSION "${OpenSSL_VERSION}")
            elseif (DEFINED OPENSSL_VERSION)
                set(NNG_OPENSSL_VERSION "${OPENSSL_VERSION}")
            else()
                set(NNG_OPENSSL_VERSION "")
            endif()
            if (NNG_OPENSSL_VERSION STREQUAL "")
                message(FATAL_ERROR
                    "NNG_REQUIRE_PKCS11_PROVIDER could not determine the OpenSSL version at configure time.")
            endif()
            if (NNG_OPENSSL_VERSION VERSION_LESS "3.0.0")
                message(FATAL_ERROR
                    "NNG_REQUIRE_PKCS11_PROVIDER requires OpenSSL >= 3.0, found ${NNG_OPENSSL_VERSION}.")
            endif()
        endif()
        nng_link_libraries_public(OpenSSL::SSL OpenSSL::Crypto)
    endif()

    nng_defines(NNG_TLS_ENGINE_INIT=nng_tls_engine_init_open)
    nng_defines(NNG_TLS_ENGINE_FINI=nng_tls_engine_fini_open)
    nng_defines(NNG_TLS_ENGINE_CHECK_PKCS11=nng_tls_engine_check_pkcs11_open)
    nng_defines(NNG_SUPP_TLS)
    nng_defines(NNG_TLS_ENGINE_OPENSSL)
endif ()
