/*
 * Copyright (C) 2013 Bastian Bloessl <bloessl@ccs-labs.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef INCLUDED_IEEE802_11_OFDM_UTILS_H
#define INCLUDED_IEEE802_11_OFDM_UTILS_H

#include <ieee802-11/api.h>
#include <ieee802-11/ofdm_mapper.h>
#include <cinttypes>
#include <iostream>

#define dout d_debug && std::cout
#define mylog(msg) do { if(d_log) *d_logger << log4cpp::Priority::INFO << msg << log4cpp::eol; } while(0);

struct MAC_DATAFRAME_HEADER {
	//protocol version, type, subtype, to_ds, from_ds, ...
	char frame_control[2];
	char duration[2];
	char address1[6];
	char address2[6];
	char address3[6];
	uint16_t sequence;
}__attribute__((packed));

/**
 * OFDM parameters
 */
class ofdm_param {
public:
	ofdm_param(Encoding e);

	// data rate
	Encoding encoding;
	// rate field of the SIGNAL header
	char     rate_field;
	// number of coded bits per sub carrier
	int      n_bpsc;
	// number of coded bits per OFDM symbol
	int      n_cbps;
	// number of data bits per OFDM symbol
	int      n_dbps;

	void print();
};

/**
 * packet specific parameters
 */
class tx_param {
public:
	tx_param(ofdm_param &ofdm, int psdu_length);
	// PSDU size in bytes
	int psdu_size;
	// number of OFDM symbols (17-11)
	int n_sym;
	// number of data bits in the DATA field, including service and padding (17-12)
	int n_data;
	// number of padding bits in the DATA field (17-13)
	int n_pad;
	int n_encoded_bits;

	void print();
};

/**
 * Given a payload, generates a MAC data frame (i.e., a PSDU) to be given
 * to the physical layer for encoding.
 *
 * \param msdu the payload for the MAC frame
 * \param msdu_size the size of the msdu in bytes
 * \param psdu pointer to a byte array where to store the MAC frame. Memory
 * will be alloced by the function
 * \param psdu_size pointer to an integer where the size of the psdu in bytes
 * will be stored
 * \param seq sequence number of the frame
 */
void generate_mac_data_frame(const char *msdu, int msdu_size, char **psdu, int *psdu_size, char seq);


void scramble(const char *input, char *out, tx_param &tx, char initial_state);

void reset_tail_bits(char *scrambled_data, tx_param &tx);

void convolutional_encoding(const char *input, char *out, tx_param &tx);

void puncturing(const char *input, char *out, tx_param &tx, ofdm_param &ofdm);

void interleave(const char *input, char *out, tx_param &tx, ofdm_param &ofdm, bool reverse = false);
void interleave(const double *input, double *out, tx_param &tx, ofdm_param &ofdm, bool reverse = false);

void split_symbols(const char *input, char *out, tx_param &tx, ofdm_param &ofdm);

void generate_signal_field(char *out, tx_param &tx, ofdm_param &ofdm);

void generate_bits(const char *psdu, char *data_bits, tx_param &tx);

#endif /* INCLUDED_IEEE802_11_OFDM_UTILS_H */