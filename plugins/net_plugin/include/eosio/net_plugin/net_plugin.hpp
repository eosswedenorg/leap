#pragma once

#include <eosio/chain/application.hpp>
#include <eosio/net_plugin/protocol.hpp>
#include <eosio/chain_plugin/chain_plugin.hpp>

namespace eosio {
   using namespace appbase;

   struct connection_status {
      string            peer;
      bool              connecting = false;
      bool              syncing    = false;
      bool              is_bp_peer = false;
      handshake_message last_handshake;
   };

   class net_plugin : public appbase::plugin<net_plugin>
   {
      public:
        net_plugin();
        virtual ~net_plugin();

        APPBASE_PLUGIN_REQUIRES((chain_plugin))
        virtual void set_program_options(options_description& cli, options_description& cfg) override;
        void handle_sighup() override;

        void plugin_initialize(const variables_map& options);
        void plugin_startup();
        void plugin_shutdown();

        string                            connect( const string& endpoint );
        string                            disconnect( const string& endpoint );
        std::optional<connection_status>  status( const string& endpoint )const;
        vector<connection_status>         connections()const;

        struct p2p_connections_metrics {
           std::size_t num_peers;
           std::size_t num_clients;
        };

        void register_update_p2p_connection_metrics(std::function<void(p2p_connections_metrics)>&&);
        void register_increment_failed_p2p_connections(std::function<void()>&&);
        void register_increment_dropped_trxs(std::function<void()>&&);

      private:
        std::shared_ptr<class net_plugin_impl> my;
   };

}

FC_REFLECT( eosio::connection_status, (peer)(connecting)(syncing)(is_bp_peer)(last_handshake) )
