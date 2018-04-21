package com.guildon.watercontainer;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;

/**
 * Created by Chris on 21/Apr/2018.
 */
public class WaterContainerView extends View {

	Paint mLinePaint;
	Paint mWallPaint;
	Paint mWaterPaint;

	char[][] mLayout;

	final int mCellSize = 20;

	public WaterContainerView(Context context) {
		super(context);

		setupPaint();
	}

	public WaterContainerView(Context context, AttributeSet set) {
		super(context, set);

		setupPaint();
	}

	public void setLayout(char[][] layout) {
		mLayout = layout;
	}


	private void setupPaint() {
		mWallPaint = new Paint();
		mWaterPaint = new Paint();
		mLinePaint = new Paint();

		mWallPaint.setColor(Color.GRAY);
		mWaterPaint.setColor(Color.CYAN);
		mLinePaint.setColor(Color.BLACK);

		mLinePaint.setStrokeWidth(2);
	}


	@Override
	protected void onDraw(Canvas canvas) {

		if (mLayout == null)
			return;

		for (int i = 0; i < mLayout.length; i++) {
			for (int j = 0; j < mLayout[i].length; j++) {
				switch (mLayout[i][j]) {
					case '#':
						canvas.drawRect(j * mCellSize, i * mCellSize, (j + 1) * mCellSize, (i + 1) * mCellSize, mWallPaint);
						break;
					case 'w':
						canvas.drawRect(j * mCellSize, i * mCellSize, (j + 1) * mCellSize, (i + 1) * mCellSize, mWaterPaint);
						break;
				}
			}
		}

		for (int i = 0; i < mLayout.length; i++) {
			canvas.drawLine(0, i * mCellSize, mLayout[i].length * mCellSize, i * mCellSize, mLinePaint);
			canvas.drawLine(i * mCellSize, 0, i * mCellSize, mLayout.length * mCellSize, mLinePaint);
		}
	}
}
