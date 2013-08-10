/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package apidemo.util;

import javax.swing.JTextField;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;

public class UpperField extends JTextField {
	int m_ival;
	double m_dval;
	
	public UpperField() {
		this( null);
	}
	
	public UpperField( int i) {
		this( null, i);
	}
	
	public UpperField( String s) {
		this( s, 7);
	}
	
	public UpperField( String s, int i) {
		super( i);
		
		setDocument( new PlainDocument() {
			@Override public void insertString(int offs, String str, AttributeSet a) throws BadLocationException {
				super.insertString(offs, str.toUpperCase(), a);
			}
		});
		
		setText( s);
	}
	
	public void setText( double v) {
		if (v == Double.MAX_VALUE || v == 0) {
			m_dval = v;
			setText( null);
		}
		else {
			super.setText( "" + v);
		}
	}

	public void setText( int v) {
		if (v == Integer.MAX_VALUE || v == 0) {
			m_ival = v;
			setText( null);
		}
		else {
			super.setText( "" + v);
		}
	}

	@Override public void setText(String t) {
		super.setText(t);
	}
	
	public double getDouble() {
		try {
			String str = super.getText();
			return str == null || str.length() == 0
				 ? m_dval : Double.parseDouble( super.getText().trim() );
		}
		catch( Exception e) {
			return 0;
		}
	}

	public int getInt() {
		try {
			String str = super.getText();
			return str == null || str.length() == 0
				 ? m_ival : Integer.parseInt( super.getText().trim() );
		}
		catch( Exception e) {
			return 0;
		}
	}
}
