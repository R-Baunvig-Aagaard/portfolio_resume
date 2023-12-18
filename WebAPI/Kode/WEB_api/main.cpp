#include <iostream>

#include <restinio/all.hpp>
#include <restinio/websocket/websocket.hpp>

#include <json_dto/pub.hpp>

#include <fmt/format.h>

//struct place ....
struct place_t 
{
	place_t() {}

	place_t(
		std::string navn,
		double lat,
		double lon )
		:	m_navn{ std::move( navn ) }
		,   m_lat{ std::move( lat ) }
		,   m_lon{ std::move( lon ) }
	{}

	std::string m_navn;
	double m_lat;
	double m_lon;

	template < typename JSON_IO >
	void
	json_io( JSON_IO & io )
	{
		io
			& json_dto::mandatory( "navn", m_navn )
			& json_dto::mandatory( "lat", m_lat )
			& json_dto::mandatory( "lon", m_lon );
			
	}
	
};

//struct time ....
struct tidDato_t 
{
	tidDato_t() {}

	tidDato_t(
		std::string dato,
		std::string klokken )
		:	m_dato{ std::move( dato ) }
		,   m_klokken{ std::move( klokken ) }
	{}

	std::string m_dato;
	std::string m_klokken;

	template < typename JSON_IO >
	void
	json_io( JSON_IO & io )
	{
		io
			& json_dto::mandatory( "dato", m_dato )
			& json_dto::mandatory( "klokken", m_klokken );
			
	}
	
};

struct weather_t 
{
	weather_t() {}
	weather_t(
		int id,
		tidDato_t tidspunkt,
		place_t places,
		double temperatur,
		int luftfugtighed )
		:	m_id{ std::move( id ) } 
		,	m_tidspunkt{std::move( tidspunkt ) }
		,	m_place{std::move( places ) }
		,   m_temp{ std::move( temperatur ) }
		,	m_luft{ std::move( luftfugtighed ) }
	{}

	int m_id;
	tidDato_t m_tidspunkt;
	place_t m_place;
	double m_temp;
	int m_luft;

	template < typename JSON_IO >
	void
	json_io( JSON_IO & io )
	{
		io
			& json_dto::mandatory( "id", m_id )
			& json_dto::mandatory( "tidspunkt", m_tidspunkt )
			& json_dto::mandatory( "place", m_place )
			& json_dto::mandatory( "temperatur", m_temp )
			& json_dto::mandatory( "luftfugtighed", m_luft );
	}

};

using weather_station_t = std::vector< weather_t >; 

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

using traits_t =
	restinio::traits_t<
		restinio::asio_timer_manager_t,
		restinio::single_threaded_ostream_logger_t,
		router_t >;

namespace rws = restinio::websocket::basic;

using ws_registry_t = std::map< std::uint64_t, rws::ws_handle_t >;

// HANDLER
class weather_handler_t
{
public :
	explicit weather_handler_t( weather_station_t & weather)
		:	m_weather( weather )
	{}

	weather_handler_t( const weather_handler_t & ) = delete;
	weather_handler_t( weather_handler_t && ) = delete;

	/*GET WEATHER DATA*/
	auto on_weather_data(
		const restinio::request_handle_t& req, rr::route_params_t ) const
	{
		auto resp = init_resp( req->create_response() );

		resp.set_body(
			"Collection of Weather data: \n" );

		for( std::size_t i = 0; i < m_weather.size(); ++i )
		{	
			//const auto & b = m_weather[ i ];
			//resp.append_body("\nID: " + b.m_id + "\n - Tidspunkt: " + b.m_tidspunkt + "\n - Place: " + b.m_place + " - Temperatur: " + b.m_temp + " - Luftfugtighed: " + b.m_luft + "\n" );
			resp.append_body("\n Converted to json \n");
			resp.append_body(json_dto::to_json(m_weather[i]) + "\n");
		}

		return resp.done();
	}

	auto on_weather_get(
		const restinio::request_handle_t& req, rr::route_params_t params )
	{
		const auto datanum = restinio::cast_to< std::uint32_t >( params[ "datanum" ] );

		auto resp = init_resp( req->create_response() );

		if( 0 != datanum && datanum <= m_weather.size() )
		{
			//const auto & b = m_weather[ datanum - 1 ];
			resp.set_body(
				"Found this data: \n ");

			//resp.append_body("\nID: " + b.m_id + "\n" + " - Place: " + b.m_place + " - Temperatur: " + b.m_temp + " - Luftfugtighed: " + b.m_luft + "\n" );
			resp.append_body("\n Converted to json \n");
			resp.append_body(json_dto::to_json(m_weather[datanum - 1 ]) + "\n");
		}
		else
		{
			resp.set_body(
				"No data match ID:# " + std::to_string( datanum ) + "\n" );
		}

		return resp.done();
	}

	auto on_date_get(
		const restinio::request_handle_t& req, rr::route_params_t params )
	{
		auto resp = init_resp( req->create_response() );
		try
		{
			auto date = restinio::utils::unescape_percent_encoding( params[ "param" ] );
			
			resp.set_body( "Weather from " + date + ":\n" );
			for( std::size_t i = 0; i < m_weather.size(); ++i )
			{
				const auto & b = m_weather[ i ];

				if( date == b.m_tidspunkt.m_dato )
				{
					resp.append_body("\n Converted to json \n");
					resp.append_body(json_dto::to_json(m_weather[i]) + "\n");

				} 
				
			}
		}
		catch( const std::exception & )
		{
			mark_as_bad_request( resp );
		}

		return resp.done();
	}

	auto on_new_weather_data(
		const restinio::request_handle_t& req, rr::route_params_t )
	{
		auto resp = init_resp( req->create_response() );
		
		try
		{	
			m_weather.emplace_back(
				json_dto::from_json< weather_t >( req->body() ));
				
		}
		catch( const std::exception & /*ex*/ )
		{
			mark_as_bad_request( resp );
		}

		return resp.done();
	}

	auto on_weather_update(
		const restinio::request_handle_t& req, rr::route_params_t params )
	{
		const auto datanum = restinio::cast_to< std::uint32_t >( params[ "datanum" ] );

		auto resp = init_resp( req->create_response() );
		
		try
		{
			auto b = json_dto::from_json< weather_t >( req->body() );

			if( 0 != datanum && datanum <= m_weather.size() )
			{
				m_weather[ datanum - 1 ] = b;
			}
			else
			{
				mark_as_bad_request( resp );
				resp.set_body( "No weather data with #" + std::to_string( datanum ) + "\n" );
			}
		}
		catch( const std::exception & /*ex*/ )
		{
			mark_as_bad_request( resp );
		}

		return resp.done();
	}

	auto on_weather_data_delete(
		const restinio::request_handle_t& req, rr::route_params_t params )
	{
		const auto datanum = restinio::cast_to< std::uint32_t >( params[ "datanum" ] );

		auto resp = init_resp( req->create_response() );

		if( 0 != datanum && datanum <= m_weather.size() )
		{
			//const auto & b = m_weather[ datanum - 1 ];
			/*resp.set_body(
				"Delete weather data #" + std::to_string( datanum ) + "ID: " + b.m_id + " - Luftfugtighed: " +
					b.m_luft + " - Temperatur: " + b.m_temp + "\n" );*/

			m_weather.erase( m_weather.begin() + ( datanum - 1 ) );
		}
		else
		{
			resp.set_body(
				"No weather data with #" + std::to_string( datanum ) + "\n" );
		}

		return resp.done();
	}	

	auto request_handler(
		const restinio::request_handle_t& req, rr::route_params_t params )
	{
		
		ws_registry_t registry;
		if( restinio::http_connection_header_t::upgrade == req->header().connection() )
			{
				auto wsh =
					rws::upgrade< traits_t >(
						*req,
						rws::activation_t::immediate,
						[ &registry ]( auto wsh, auto m ){
							if( rws::opcode_t::text_frame == m->opcode() ||
								rws::opcode_t::binary_frame == m->opcode() ||
								rws::opcode_t::continuation_frame == m->opcode() )
							{
								wsh->send_message( *m );
							}
							else if( rws::opcode_t::ping_frame == m->opcode() )
							{
								auto resp = *m;
								resp.set_opcode( rws::opcode_t::pong_frame );
								wsh->send_message( resp );
							}
							else if( rws::opcode_t::connection_close_frame == m->opcode() )
							{
								registry.erase( wsh->connection_id() );
							}
						} );

				registry.emplace( wsh->connection_id(), wsh );
				init_resp(req->create_response()).done();
				return restinio::request_accepted();
			}

			return restinio::request_rejected();
		
	}

	

//HANDLER DONE

private :
	weather_station_t & m_weather;
	
	template < typename RESP >
	static RESP
	init_resp( RESP resp )
	{
		resp
			.append_header( "Server", "RESTinio sample server /v.0.6" )
			.append_header_date_field()
			.append_header( "Content-Type", "text/plain; charset=utf-8" );

		return resp;
	}

	template < typename RESP >
	static void
	mark_as_bad_request( RESP & resp )
	{
		resp.header().status_line( restinio::status_bad_request() );
	}
};


//SERVER HANDLER

auto server_handler( weather_station_t & weather_collection) 
{
	auto router = std::make_unique< router_t >();
	auto handler = std::make_shared< weather_handler_t >( std::ref(weather_collection) );

	auto by = [&]( auto method ) {
		using namespace std::placeholders;
		return std::bind( method, handler, _1, _2 );
	};

	
	auto method_not_allowed = []( const auto & req, auto ) {
			return req->create_response( restinio::status_method_not_allowed() )
					.connection_close()
					.done();
		};

	// Handlers for '/' path.
	router->http_get( "/", by( &weather_handler_t::on_weather_data ) ); 
	router->http_post( "/", by( &weather_handler_t::on_new_weather_data ) );

	// Disable all other methods for '/'.
	router->add_handler(
			restinio::router::none_of_methods(
					restinio::http_method_get(), restinio::http_method_post() ),
			"/", method_not_allowed );

	// Handler for '/single/:param' path.
	router->http_get( "/date/:param", by( &weather_handler_t::on_date_get ) );

	// Disable all other methods for '/single/:param'.
	router->add_handler(
			restinio::router::none_of_methods( restinio::http_method_get() ),
			"/date/:param", method_not_allowed );


	// Router websocket
	router->http_get("/chat", by( &weather_handler_t::request_handler ) );

	
	// Handlers for '/:datanum' path.
	router->http_get(
			R"(/:datanum(\d+))",
			by( &weather_handler_t::on_weather_get ) );
	router->http_put(
			R"(/:datanum(\d+))",
			by( &weather_handler_t::on_weather_update ) );
	router->http_delete(
			R"(/:datanum(\d+))",
			by( &weather_handler_t::on_weather_data_delete ) );
	/*router->http_post(
			R"(/new/:id(\d+).:place.:temp(\d+).:luftfugtighed(\d+))", by( &weather_handler_t::on_new_weather_data ) );
	*/
	// Disable all other methods for '/:datanum'.
	router->add_handler(
			restinio::router::none_of_methods(
					restinio::http_method_get(),
					restinio::http_method_post(),
					restinio::http_method_delete() ),
			R"(/:datanum(\d+))", method_not_allowed );



	return router;
}

int main()
{
	using namespace std::chrono;

	try
	{
		using traits_t =
			restinio::traits_t<
				restinio::asio_timer_manager_t,
				restinio::single_threaded_ostream_logger_t,
				router_t >;

		/*{
		auto weather1 = json_dto::from_json< weather_t >( json_data );
		}*/

		tidDato_t tid1(
			"17-11-2021",
			"17:52"
		);

		place_t place1(
			"Aarhus",
			13.692,
			19.438
		);

		weather_t weather1(//vejrstation
			1,
			tid1,
			place1,
			13.1,
			70
		);

		//std::cout << "\nSerialized to JSON:\n" << json_dto::to_json( weather1 ) << std::endl;
		//weather1 = json_dto::to_json( weather1 );
		
		/*{
		auto weather1 = json_dto::from_json< weather_t >( json_data );
		}*/

		tidDato_t tid2(
			"17-10-2021",
			"12:52"
		);

		place_t place2(
			"Viborg",
			10.452,
			19.333
		);

		weather_t weather2( //vejrstation
			2,
			tid2,
			place2,
			22.8,
			99
		);
		
	

		weather_station_t weatherCollection{
			weather1,
			weather2
		};

		//ws_registry_t registry;
		restinio::run(
			restinio::on_this_thread< traits_t >()
				.address( "localhost" )
				.request_handler( server_handler( weatherCollection ) ) //collection
				.read_next_http_message_timelimit( 10s )
				.write_http_response_timelimit( 1s )
				.handle_request_timeout( 1s ) );
	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
