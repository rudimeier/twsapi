/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package apidemo;

import java.awt.BorderLayout;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JDialog;
import javax.swing.JFrame;


import apidemo.util.HtmlButton;

import com.ib.controller.NewContract;

class ContractDlg extends JDialog {
	private int m_conId;
//		private UpperField m_includeExpired = new UpperField( 7);
//	    private String m_secIdType;        // CUSIP;SEDOL;ISIN;RIC
//	    private String m_secId;
//	    private String m_comboLegsDescrip; // received in open order version 14 and up for all combos
//	    private Vector<ComboLeg> m_comboLegs = new Vector<ComboLeg>();
//	    private UnderComp m_underComp;
	ContractPanel m_contractPanel;
    
    ContractDlg( JFrame f, NewContract c) {
    	super( f, true);
    	
    	m_contractPanel = new ContractPanel( c);
    	
    	setLayout( new BorderLayout() );
    	
    	
    	HtmlButton ok = new HtmlButton( "OK") {
			@Override public void actionPerformed() {
				onOK();
			}
		};
		ok.setHorizontalAlignment(HtmlButton.CENTER);

		m_contractPanel.addKeyListener( new KeyListener() {
			@Override public void keyTyped(KeyEvent e) {
				System.out.println( "lkj");
			}
			
			@Override public void keyReleased(KeyEvent e) {
			}
			
			@Override public void keyPressed(KeyEvent e) {
			}
		});
    	
    	add( m_contractPanel);
    	add( ok, BorderLayout.SOUTH);
    	pack();
    }

	private void onOK() {
		m_contractPanel.onOK();
		setVisible( false);
	}
}
