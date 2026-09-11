/*
 * idevice.h
 * Device discovery and communication interface -- header file.
 *
 * Copyright (c) 2008 Zach C. All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __DEVICE_H
#define __DEVICE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/* legacy branch: idevice.c's own connection-layer SSL/TLS is independently
 * selectable from the plain HAVE_OPENSSL used elsewhere in this library
 * (common/userpref.c's cert generation) via --with-ssl-implementation (see
 * configure.ac) -- LIBIMOBILEDEVICE_SSL_IMPLEMENTATION_OPENSSL/_GNUTLS
 * mirror the original two choices exactly; _WOLFSSL is new, added because
 * neither OpenSSL nor GnuTLS builds this project links against can
 * negotiate real SSLv3 (see idevice_connection_enable_ssl() for the full
 * story). wolfssl/options.h must be processed before any other wolfSSL
 * header (its headers aren't self-contained the way OpenSSL's are). */
#if defined(LIBIMOBILEDEVICE_SSL_IMPLEMENTATION_WOLFSSL)
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#elif defined(LIBIMOBILEDEVICE_SSL_IMPLEMENTATION_GNUTLS)
#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#else
#include <openssl/ssl.h>
#endif

#ifdef WIN32
#define LIBIMOBILEDEVICE_API __declspec( dllexport )
#else
#ifdef HAVE_FVISIBILITY
#define LIBIMOBILEDEVICE_API __attribute__((visibility("default")))
#else
#define LIBIMOBILEDEVICE_API
#endif
#endif

#include "common/userpref.h"
#include "libimobiledevice/libimobiledevice.h"

#define DEVICE_VERSION(maj, min, patch) (((maj & 0xFF) << 16) | ((min & 0xFF) << 8) | (patch & 0xFF))

struct ssl_data_private {
#if defined(LIBIMOBILEDEVICE_SSL_IMPLEMENTATION_WOLFSSL)
	WOLFSSL *session;
	WOLFSSL_CTX *ctx;
#elif defined(LIBIMOBILEDEVICE_SSL_IMPLEMENTATION_GNUTLS)
	gnutls_certificate_credentials_t certificate;
	gnutls_session_t session;
	gnutls_x509_privkey_t root_privkey;
	gnutls_x509_crt_t root_cert;
	gnutls_x509_privkey_t host_privkey;
	gnutls_x509_crt_t host_cert;
#else
	SSL *session;
	SSL_CTX *ctx;
#endif
};
typedef struct ssl_data_private *ssl_data_t;

struct idevice_connection_private {
	idevice_t device;
	enum idevice_connection_type type;
	void *data;
	ssl_data_t ssl_data;
};

struct idevice_private {
	char *udid;
	uint32_t mux_id;
	enum idevice_connection_type conn_type;
	void *conn_data;
	int version;
};

#endif
