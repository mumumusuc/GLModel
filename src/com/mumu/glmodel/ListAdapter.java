package com.mumu.glmodel;

import java.util.List;

import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

public class ListAdapter extends BaseAdapter {

	List<String> mList;

	public void setList(List<String> list) {
		mList = list;
	}

	@Override
	public int getCount() {
		return mList == null ? 0 : mList.size();
	}

	@Override
	public Object getItem(int arg0) {
		return mList == null ? null : mList.get(arg0);
	}

	@Override
	public long getItemId(int arg0) {
		return 0;
	}

	@Override
	public View getView(int arg0, View arg1, ViewGroup arg2) {
		if (!(arg1 instanceof TextView)) {
			arg1 = new TextView(arg2.getContext());
			((TextView) arg1).setTextSize(38);
		}
		((TextView) arg1).setText(mList.get(arg0));
		return arg1;
	}

}
