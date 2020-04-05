#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
// Message table
struct [[eosio::table("testkits"), eosio::contract("rymedilog")]] testkits {
    uint64_t    id       = {}; // Non-0
     uint64_t    reply_to = {}; // Non-0 if this is a reply
    std::string uuid     = {};
    std::string testkitid  = {};

    uint64_t primary_key() const { return id; }
    uint64_t get_reply_to() const { return reply_to; }
};

using message_table = eosio::multi_index<
    "testkits"_n, testkits, eosio::indexed_by<"by.reply.to"_n, eosio::const_mem_fun<testkits, uint64_t, &testkits::get_reply_to>>>;

// The contract
class rymedilog : eosio::contract {
  public:
    // Use contract's constructor
    using contract::contract;

    // Post a message
    [[eosio::action]] void post(uint64_t id,uint64_t reply_to, std::string& uuid, const std::string& testkitid) {
        message_table table{get_self(), 0};

        // Create an ID if user didn't specify one
        eosio::check(id < 1'000'000'000ull, "user-specified id is too big");
        if (!id)
            id = std::max(table.available_primary_key(), 1'000'000'000ull);

// Check reply_to exists
        if (reply_to)
            table.get(reply_to);
        // Record the message
        table.emplace(get_self(), [&](auto& testkits) {
            testkits.id       = id;
            testkits.reply_to = reply_to;
            testkits.uuid     = uuid;
            testkits.testkitid  = testkitid;
        });
    }
    [[eosio::action]] auto find(uint64_t id, std::string secret) {
        message_table ttabs{get_self(), 0};
// Check user
        // require_auth(user);
        if(secret != "rymedilog")
            return;
        // Create an ID if user didn't specify one
        auto iter=ttabs.find(id);
        eosio::print(iter->testkitid);

    }
};
