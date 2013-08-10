/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package apidemo.util;

import javax.swing.JComboBox;

public class TCombo<T> extends JComboBox {
	public TCombo( T... strs) {
		super( strs);
	}

	public String getText() {
		return getSelectedItem() == null ? null : getSelectedItem().toString();
	}
	
	@Override public T getSelectedItem() {
		return (T)super.getSelectedItem();
	}
}
