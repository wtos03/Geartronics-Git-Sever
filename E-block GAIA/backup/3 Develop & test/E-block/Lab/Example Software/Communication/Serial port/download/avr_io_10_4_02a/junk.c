          switch( rx_buffer[rx_out] )
          {
            case 0x41: //A
              parse_state = PARSE_STATE_READ_ALL_AD;
              break;

            case 0x44: //D
              parse_state = PARSE_STATE_SET_DDR;
              break;

            case 0x61: //a
              parse_state = PARSE_STATE_READ_AD;
              break;
              
						case 0x52: //R
              parse_state = PARSE_STATE_READ_ALL_DIO;
              break;
            
            case 0x57: //W
              parse_state = PARSE_STATE_WRITE_ALL_DIO;
              break;

            case 0x72: //r
              parse_state = PARSE_STATE_READ_DIO;
              break;
            
            case 0x77: //w
              parse_state = PARSE_STATE_WRITE_DIO;
              break;

						default:
							parse_state = PARSE_STATE_STX;
							break;
          }
          break;

				case PARSE_STATE_READ_ALL_AD:
				  tx_buffer[2] = 
				  tx_buffer[3] = 
				  tx_buffer[4] = 
				  tx_buffer[5] = 
				  tx_buffer[6] = 0x03;
				  response_numbytes = 7;
					break;
					
				case PARSE_STATE_READ_AD:
				  tx_buffer[2] = 
				  tx_buffer[3] = 0x03;
				  response_numbytes = 4;
					break;
					
				case PARSE_STATE_READ_ALL_DIO:
				  tx_buffer[2] = inp( PORTA );
				  tx_buffer[3] = inp( PORTB );
				  tx_buffer[4] = inp( PORTC );
				  tx_buffer[5] = inp( PORTD );
				  tx_buffer[6] = 0x03;
				  response_numbytes = 7;
					break;
					
				case PARSE_STATE_READ_DIO:
				  tx_buffer[2] = 
					tx_buffer[3] = 0x03;
				  response_numbytes = 4;
					break;

        case PARSE_STATE_WRITE_DIO:
          parm[parm_index] = rx_buffer[rx_out];
          if( ++parm_index == 2 )
          {
            port_addr = GetPortId( parm[0] );
            if( port_addr != 0 )
            {
              outp( parm[1], port_addr );
            }

            parse_state = PARSE_STATE_ETX;
          }
          break;

        case PARSE_STATE_SET_DDR:
          parm[parm_index] = rx_buffer[rx_out];
          if( ++parm == 4 )
          {
            outp( parm[0], DDRA );
            outp( parm[1], DDRB );
            outp( parm[2], DDRC );
            outp( parm[3], DDRD );

            parse_state = PARSE_STATE_ETX;
          }
          break;

        case PARSE_STATE_WRITE_ALL_DIO:
          data_store[data_index] = rx_buffer[rx_out];
          if( ++data_index == 4 )
          {
            outp( data_store[0], PORTA );
            outp( data_store[1], PORTB );
            outp( data_store[2], PORTC );
            outp( data_store[3], PORTD );

            parse_state = PARSE_STATE_ETX;
          }
          break;
