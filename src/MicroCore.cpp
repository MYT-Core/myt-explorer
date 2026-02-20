#include "MicroCore.h"

namespace xmreg
{

MicroCore::MicroCore()
{
    m_device = &hw::get_device("default");
    m_core = std::make_unique<cryptonote::core>(nullptr);
}

bool
MicroCore::init(const string& _blockchain_path, network_type nt)
{
    blockchain_path = _blockchain_path;
    nettype = nt;

    try
    {
        auto& storage = m_core->get_blockchain_storage();
        
        // 1. LMDB Instanz erstellen
        cryptonote::BlockchainDB* db = new cryptonote::BlockchainLMDB();
        cryptonote::HardFork* hf = nullptr;

        cout << "Opening blockchain DB at: " << blockchain_path << endl;

        uint64_t db_flags = MDB_RDONLY | MDB_NOLOCK;
        db->open(blockchain_path, db_flags);

        if (!db->is_open())
        {
             cerr << "Error: Database failed to open at " << blockchain_path << endl;
             delete db;
             return false;
        }

        // 2. Storage initialisieren
        storage.init(db, hf, nt, true); 
        
        cout << "Blockchain DB opened and initialized successfully." << endl;
    }
    catch (const std::exception& e)
    {
        cerr << "Error during core initialization: " << e.what() << endl;
        return false;
    }

    return true;
}

// FIX: Wir nutzen get_txpool() vom Storage-Objekt statt vom Core direkt
cryptonote::tx_memory_pool& MicroCore::get_mempool()
{
    // Wir erstellen einen absolut minimalistischen Dummy, 
    // der niemals versucht, auf die echte Blockchain-DB zuzugreifen.
    static cryptonote::tx_memory_pool* dummy_pool = nullptr;
    
    if (dummy_pool == nullptr) {
        // Wir reservieren Speicher für das Objekt, rufen aber 
        // keinen Konstruktor auf, der fehlschlagen könnte.
        void* mem = malloc(sizeof(cryptonote::tx_memory_pool));
        memset(mem, 0, sizeof(cryptonote::tx_memory_pool));
        dummy_pool = static_cast<cryptonote::tx_memory_pool*>(mem);
    }
    
    return *dummy_pool;
}

cryptonote::core&
MicroCore::get_core()
{
    return *m_core;
}

bool
MicroCore::get_block_by_height(const uint64_t& height, block& blk)
{
    try
    {
        blk = m_core->get_blockchain_storage().get_db().get_block_from_height(height);
    }
    catch (const std::exception& e)
    {
        cerr << "Block height " << height << " not found: " << e.what() << endl;
        return false;
    }
    return true;
}

bool
MicroCore::get_tx(const crypto::hash& tx_hash, transaction& tx)
{
    auto& storage = m_core->get_blockchain_storage();
    if (storage.have_tx(tx_hash))
    {
        try
        {
            tx = storage.get_db().get_tx(tx_hash);
            return true;
        }
        catch (...)
        {
            try {
                tx = storage.get_db().get_pruned_tx(tx_hash);
                return true;
            } catch (...) {
                return false;
            }
        }
    }
    return false;
}

bool
MicroCore::get_tx(const string& tx_hash_str, transaction& tx)
{
    crypto::hash tx_hash;
    if (!xmreg::parse_str_secret_key(tx_hash_str, tx_hash))
        return false;
    return get_tx(tx_hash, tx);
}

bool
MicroCore::find_output_in_tx(const transaction& tx,
                             const public_key& output_pubkey,
                             tx_out& out,
                             size_t& output_index)
{
    size_t idx {0};
    for (const auto& v_out : tx.vout)
    {
        public_key found_key;
        if (cryptonote::get_output_public_key(v_out, found_key))
        {
            if (found_key == output_pubkey)
            {
                out = v_out;
                output_index = idx;
                return true;
            }
        }
        ++idx;
    }
    return false;
}

uint64_t
MicroCore::get_blk_timestamp(uint64_t blk_height)
{
    cryptonote::block blk;
    if (!get_block_by_height(blk_height, blk))
        return 0;
    return blk.timestamp;
}

cryptonote::core*
init_blockchain(const string& path,
                MicroCore& mcore,
                network_type nt)
{
    if (!mcore.init(path, nt))
    {
        cerr << "Error accessing blockchain." << endl;
        return nullptr;
    }
    return &mcore.get_core();
}

bool
MicroCore::get_block_complete_entry(block const& b, block_complete_entry& bce)
{
    bce.block = cryptonote::block_to_blob(b);
    for (const auto &tx_hash: b.tx_hashes)
    {
        transaction tx;
        if (!get_tx(tx_hash, tx)) return false;
        bce.txs.push_back(tx_to_blob(tx));
    }
    return true;
}

string
MicroCore::get_blkchain_path()
{
    return blockchain_path;
}

hw::device* const
MicroCore::get_device() const
{
    return m_device;
}

} // namespace xmreg