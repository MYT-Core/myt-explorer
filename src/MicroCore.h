#ifndef XMREG01_MICROCORE_H
#define XMREG01_MICROCORE_H

#include <iostream>
#include <memory>

#include "cryptonote_core/cryptonote_core.h"
#include "monero_headers.h"
#include "tools.h"

namespace xmreg
{
    using namespace cryptonote;
    using namespace crypto;
    using namespace std;

    class MicroCore
    {
        string blockchain_path;
        hw::device* m_device;
        std::unique_ptr<cryptonote::core> m_core;
        network_type nettype;

    public:
        MicroCore();

        /**
         * Initialisiert den Monero-Core im Read-Only Modus.
         */
        bool
        init(const string& _blockchain_path, network_type nt);

        /**
         * Gibt eine Referenz auf das Core-Objekt zurück.
         */
        cryptonote::core&
        get_core();

        /**
         * NEU: Gibt eine Referenz auf den Mempool zurück.
         * Wichtig für MempoolStatus.cpp
         */
        cryptonote::tx_memory_pool&
        get_mempool();

        /**
         * Holt einen Block anhand seiner Höhe aus der DB.
         */
        bool
        get_block_by_height(const uint64_t& height, block& blk);

        /**
         * Holt eine Transaktion anhand ihres Hash-Werts.
         */
        bool
        get_tx(const crypto::hash& tx_hash, transaction& tx);

        /**
         * Überladene Version, die einen String-Hash akzeptiert.
         */
        bool
        get_tx(const string& tx_hash, transaction& tx);

        /**
         * Sucht einen bestimmten Output (Public Key) in einer Transaktion.
         */
        bool
        find_output_in_tx(const transaction& tx,
                          const public_key& output_pubkey,
                          tx_out& out,
                          size_t& output_index);

        /**
         * Gibt den Zeitstempel eines Blocks zurück.
         */
        uint64_t
        get_blk_timestamp(uint64_t blk_height);

        /**
         * Erstellt einen kompletten Block-Eintrag (inkl. Transaktions-Blobs).
         */
        bool
        get_block_complete_entry(block const& b, block_complete_entry& bce);

        /**
         * Gibt den Pfad zur Blockchain-DB zurück.
         */
        string
        get_blkchain_path();

        /**
         * Gibt das Hardware-Device zurück (für kryptographische Operationen).
         */
        hw::device* const
        get_device() const;
    };

    /**
     * Globale Helper-Funktion zur Initialisierung des gesamten Systems.
     */
    cryptonote::core*
    init_blockchain(const string& path,
                    MicroCore& mcore,
                    network_type nt);
}

#endif // XMREG01_MICROCORE_H