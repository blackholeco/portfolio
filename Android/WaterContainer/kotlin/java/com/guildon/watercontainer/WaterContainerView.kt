package com.guildon.watercontainer

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.View

/**
 * Created by Chris Allen on 14/Sep/2018.
 */
class WaterContainerView(context: Context, attrs: AttributeSet) : View(context, attrs) {

	private val linePaint = Paint()
	private val wallPaint = Paint()
	private val waterPaint = Paint()

	private var layoutWidth: Int = 0
	private var layoutHeight: Int = 0

	private lateinit var layout: Array<Char>

	private val cellSize = 20.0f

	init {
		linePaint.color = Color.BLACK
		linePaint.strokeWidth = 2.0f

		wallPaint.color = Color.GRAY
		waterPaint.color = Color.CYAN
	}


	fun setLayout(mLayout: Array<Char>, mWidth: Int, mHeight: Int) {
		layout = mLayout

		layoutWidth = mWidth
		layoutHeight = mHeight
	}


	override fun onDraw(canvas: Canvas) {

		for (i in 0 until layoutHeight) {
			for (j in 0 until layoutWidth) {
				when (layout[((layoutHeight - 1 - i) * layoutWidth) + j]) {
					'#' -> canvas.drawRect(j * cellSize, i * cellSize, (j + 1) * cellSize, (i + 1) * cellSize, wallPaint)
					'w' -> canvas.drawRect(j * cellSize, i * cellSize, (j + 1) * cellSize, (i + 1) * cellSize, waterPaint)
				}
			}
		}

		for (i in 1 until layoutHeight)
			canvas.drawLine(0.0f, i * cellSize, layoutWidth * cellSize, i * cellSize, linePaint)

		for(i in 0 until layoutWidth)
			canvas.drawLine(i * cellSize, 0.0f, i * cellSize, layoutHeight * cellSize, linePaint)
	}


}