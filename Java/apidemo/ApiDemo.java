/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package apidemo;


import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Rectangle;
import java.util.ArrayList;

import javax.swing.Box;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;
import javax.swing.border.EmptyBorder;

import apidemo.util.HtmlButton;
import apidemo.util.NewLookAndFeel;
import apidemo.util.NewTabbedPanel;
import apidemo.util.VerticalPanel;

import com.ib.controller.ApiConnection.ILogger;
import com.ib.controller.ApiController;
import com.ib.controller.ApiController.IBulletinHandler;
import com.ib.controller.ApiController.IConnectionHandler;
import com.ib.controller.ApiController.ITimeHandler;
import com.ib.controller.Formats;
import com.ib.controller.Types.NewsType;

public class ApiDemo implements IConnectionHandler {
	static { NewLookAndFeel.register(); }
	static ApiDemo INSTANCE = new ApiDemo();

	private final JTextArea m_inLog = new JTextArea();
	private final JTextArea m_outLog = new JTextArea();
	private final Logger m_inLogger = new Logger( m_inLog);
	private final Logger m_outLogger = new Logger( m_outLog);
	private final ApiController m_controller = new ApiController( this, m_inLogger, m_outLogger);
	private final ArrayList<String> m_acctList = new ArrayList<String>();
	private final JFrame m_frame = new JFrame();
	private final NewTabbedPanel m_tabbedPanel = new NewTabbedPanel(true);
	private final ConnectionPanel m_connectionPanel = new ConnectionPanel();
	private final MarketDataPanel m_mktDataPanel = new MarketDataPanel();
	private final ContractInfoPanel m_contractInfoPanel = new ContractInfoPanel();
	private final TradingPanel m_tradingPanel = new TradingPanel();
	private final AccountInfoPanel m_acctInfoPanel = new AccountInfoPanel();
	private final OptionsPanel m_optionsPanel = new OptionsPanel();
	private final AdvisorPanel m_advisorPanel = new AdvisorPanel();
	private final ComboPanel m_comboPanel = new ComboPanel();
	private final StratPanel m_stratPanel = new StratPanel();
	private final JTextArea m_msg = new JTextArea();

	// getter methods
	public ArrayList<String> accountList() 	{ return m_acctList; }
	public ApiController controller() 		{ return m_controller; }
	public JFrame frame() 					{ return m_frame; }

	public static void main(String[] args) {
		INSTANCE.run();
	}
	
	private void run() {
		m_tabbedPanel.addTab( "Connection", m_connectionPanel);
		m_tabbedPanel.addTab( "Market Data", m_mktDataPanel);
		m_tabbedPanel.addTab( "Trading", m_tradingPanel);
		m_tabbedPanel.addTab( "Account Info", m_acctInfoPanel);
		m_tabbedPanel.addTab( "Options", m_optionsPanel);
		m_tabbedPanel.addTab( "Combos", m_comboPanel);
		m_tabbedPanel.addTab( "Contract Info", m_contractInfoPanel);
		m_tabbedPanel.addTab( "Advisor", m_advisorPanel);
		// m_tabbedPanel.addTab( "Strategy", m_stratPanel); in progress
			
		m_msg.setEditable( false);
		m_msg.setLineWrap( true);
		JScrollPane msgScroll = new JScrollPane( m_msg);
		msgScroll.setPreferredSize( new Dimension( 10000, 120) );

		JScrollPane outLogScroll = new JScrollPane( m_outLog);
		outLogScroll.setPreferredSize( new Dimension( 10000, 120) );

		JScrollPane inLogScroll = new JScrollPane( m_inLog);
		inLogScroll.setPreferredSize( new Dimension( 10000, 120) );

		NewTabbedPanel bot = new NewTabbedPanel();
		bot.addTab( "Messages", msgScroll);
		bot.addTab( "Log (out)", outLogScroll);
		bot.addTab( "Log (in)", inLogScroll);
		
        m_frame.add( m_tabbedPanel);
        m_frame.add( bot, BorderLayout.SOUTH);
        m_frame.setSize( 1024, 768);
        m_frame.setVisible( true);
        m_frame.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE);
        
        // make initial connection to local host, port 7496, client id 0
		m_controller.connect( "127.0.0.1", 7496, 0);
    }
	
	@Override public void connected() {
		show( "connected");
		m_connectionPanel.m_status.setText( "connected");
		
		m_controller.reqCurrentTime( new ITimeHandler() {
			@Override public void currentTime(long time) {
				show( "Server date/time is " + Formats.fmtDate(time * 1000) );
			}
		});
		
		m_controller.reqBulletins( true, new IBulletinHandler() {
			@Override public void bulletin(int msgId, NewsType newsType, String message, String exchange) {
				String str = String.format( "Received bulletin:  type=%s  exchange=%s", newsType, exchange);
				show( str);
				show( message);
			}
		});
	}
	
	@Override public void disconnected() {
		show( "disconnected");
		m_connectionPanel.m_status.setText( "disconnected");
	}

	@Override public void accountList(ArrayList<String> list) {
		show( "Received account list");
		m_acctList.clear();
		m_acctList.addAll( list);
	}

	@Override public void show( final String str) {
		SwingUtilities.invokeLater( new Runnable() {
			@Override public void run() {
				m_msg.append(str);
				m_msg.append( "\n\n");
				
				Dimension d = m_msg.getSize();
				m_msg.scrollRectToVisible( new Rectangle( 0, d.height, 1, 1) );
			}
		});
	}

	@Override public void error(Exception e) {
		show( e.toString() );
	}
	
	@Override public void message(int id, int errorCode, String errorMsg) {
		show( id + " " + errorCode + " " + errorMsg);
	}
	
	private class ConnectionPanel extends JPanel {
		private final JTextField m_host = new JTextField(7);
		private final JTextField m_port = new JTextField( "7496", 7);
		private final JTextField m_clientId = new JTextField("0", 7);
		private final JLabel m_status = new JLabel("Disconnected");
		
		public ConnectionPanel() {
			HtmlButton connect = new HtmlButton("Connect") {
				@Override public void actionPerformed() {
					onConnect();
				}
			};

			HtmlButton disconnect = new HtmlButton("Disconnect") {
				@Override public void actionPerformed() {
					m_controller.disconnect();
				}
			};
			
			JPanel p1 = new VerticalPanel();
			p1.add( "Host", m_host);
			p1.add( "Port", m_port);
			p1.add( "Client ID", m_clientId);
			
			JPanel p2 = new VerticalPanel();
			p2.add( connect);
			p2.add( disconnect);
			p2.add( Box.createVerticalStrut(20));
			
			JPanel p3 = new VerticalPanel();
			p3.setBorder( new EmptyBorder( 20, 0, 0, 0));
			p3.add( "Connection status: ", m_status);
			
			JPanel p4 = new JPanel( new BorderLayout() );
			p4.add( p1, BorderLayout.WEST);
			p4.add( p2);
			p4.add( p3, BorderLayout.SOUTH);

			setLayout( new BorderLayout() );
			add( p4, BorderLayout.NORTH);
		}

		protected void onConnect() {
			int port = Integer.parseInt( m_port.getText() );
			int clientId = Integer.parseInt( m_clientId.getText() );
			m_controller.connect( m_host.getText(), port, clientId);
		}
	}
	
	private static class Logger implements ILogger {
		final private JTextArea m_area;

		Logger( JTextArea area) {
			m_area = area;
		}

		@Override public void log(final String str) {
			SwingUtilities.invokeLater( new Runnable() {
				@Override public void run() {
//					m_area.append(str);
//					
//					Dimension d = m_area.getSize();
//					m_area.scrollRectToVisible( new Rectangle( 0, d.height, 1, 1) );
				}
			});
		}
	}
}

// do clearing support
// change from "New" to something else
// more dn work, e.g. deltaNeutralValidation
// add a "newAPI" signature
// probably should not send F..A position updates to listeners, at least not to API; also probably not send FX positions; or maybe we should support that?; filter out models or include it 
// finish or remove strat panel
// check all ps
// must allow normal summary and ledger at the same time
// you could create an enum for normal account events and pass segment as a separate field
// check pacing violation
// newticktype should break into price, size, and string?
// give "already subscribed" message if appropriate

// BUGS
// When API sends multiple snapshot requests, TWS sends error "Snapshot exceeds 100/sec" even when it doesn't
// When API requests SSF contracts, TWS sends both dividend protected and non-dividend protected contracts. They are indistinguishable except for having different conids.
// Fundamentals financial summary works from TWS but not from API 
// When requesting fundamental data for IBM, the data is returned but also an error
// The "Request option computation" method seems to have no effect; no data is ever returned
// When an order is submitted with the "End time" field, it seems to be ignored; it is not submitted but also no error is returned to API.
// Most error messages from TWS contain the class name where the error occurred which gets garbled to gibberish during obfuscation; the class names should be removed from the error message 
// If you exercise option from API after 4:30, TWS pops up a message; TWS should never popup a message due to an API request
// TWS did not desubscribe option vol computation after API disconnect
// Several error message are misleading or completely wrong, such as when upperRange field is < lowerRange
// Submit a child stop with no stop price; you get no error, no rejection
// When a child order is transmitted with a different contract than the parent but no hedge params it sort of works but not really, e.g. contract does not display at TWS, but order is working
// Switch between frozen and real-time quotes is broken; e.g.: request frozen quotes, then realtime, then request option market data; you don't get bid/ask; request frozen, then an option; you don't get anything
// TWS pops up mkt data warning message in response to api order

// API/TWS Changes
// we should add underConid for sec def request sent API to TWS so option chains can be requested properly
// reqContractDetails needs primary exchange, currently only takes currency which is wrong; all requests taking Contract should be updated
// reqMktDepth and reqContractDetails does not take primary exchange but it needs to; ideally we could also pass underConid in request
// scanner results should return primary exchange
// the check margin does not give nearly as much info as in TWS
// need clear way to distinguish between order reject and warning

// API Improvements
// add logging support
// we need to add dividendProt field to contract description
// smart live orders should be getting primary exchange sent down

// TWS changes
// TWS sends acct update time after every value; not necessary
// support stop price for trailing stop order (currently only for trailing stop-limit)
// let TWS come with 127.0.0.1 enabled, same is IBG
// we should default to auto-updating client zero with new trades and open orders

// NOTES TO USERS
// you can get all orders and trades by setting "master id" in the TWS API config
// reqManagedAccts() is not needed because managed accounts are sent down on login
// TickType.LAST_TIME comes for all top mkt data requests
// all option ticker requests trigger option model calc and response
// DEV: All Box layouts have max size same as pref size; but center border layout ignores this
// DEV: Box layout grows items proportionally to the difference between max and pref sizes, and never more than max size

//TWS sends error "Snapshot exceeds 100/sec" even when it doesn't; maybe try flush? or maybe send 100 then pause 1 second? this will take forever; i think the limit needs to be increased

//req open orders can only be done by client 0 it seems; give a message
//somehow group is defaulting to EqualQuantity if not set; seems wrong
//i frequently see order canceled - reason: with no text
//Missing or invalid NonGuaranteed value. error should be split into two messages
//Rejected API order is downloaded as Inactive open order; rejected orders should never be sen
//Submitting an initial stop price for trailing stop orders is supported only for trailing stop-limit orders; should be supported for plain trailing stop orders as well 
//EMsgReqOptCalcPrice probably doesn't work since mkt data code was re-enabled
//barrier price for trail stop lmt orders why not for trail stop too?
//All API orders default to "All" for F; that's not good
