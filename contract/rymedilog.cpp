#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
// Message table
struct [[eosio::table("testkits"), eosio::contract("rymedilog")]] testkits {
    uint64_t    id           = {}; // Non-0
    std::string uuid         = {};
    std::string testkitidhash = {};
    std::string datahash     = {};

    uint64_t primary_key() const { return id; }
};

typedef eosio::multi_index<"testkits"_n, testkits> test_unit_table;

// The contract
class rymedilog : eosio::contract {
  public:
    // Use contract's constructor
    using contract::contract;

    // log a testkit
    [[eosio::action]] void logtestkit(uint64_t id, std::string& datahash, const std::string& uuid, const std::string& testkitidhash, const std::string& secret) {
        test_unit_table table{get_self(), 0};

        // Create an ID if user didn't specify one
        eosio::check(id < 1'000'000'000ull, "user-specified id is too big");
        if (!id)
            id = std::max(table.available_primary_key(), 1'000'000'000ull);

        // Check secret
        if (secret != "hYx9By6d3KZz7HqJ")
            return;
        // Record the message
        table.emplace(get_self(), [&](auto& testkits) {
            testkits.id            = id;
            testkits.datahash      = datahash;
            testkits.uuid          = uuid;
            testkits.testkitidhash = testkitidhash;
        });
    }
    [[eosio::action]] auto find(uint64_t id, std::string secret) {
        test_unit_table table{get_self(), 0};
        // Check user
        // require_auth(user);
        if (secret != "hYx9By6d3KZz7HqJ")
            return;
        // Create an ID if user didn't specify one
        auto iter = table.find(id);
        eosio::print(iter->testkitidhash);
    }
};
