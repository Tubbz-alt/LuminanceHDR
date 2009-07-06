/*
 * This file is a part of Qtpfsgui package.
 * ---------------------------------------------------------------------- 
 * Copyright (C) 2006,2007 Giuseppe Rota
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ---------------------------------------------------------------------- 
 *
 * Original Work
 * @author Giuseppe Rota <grota@users.sourceforge.net>
 * Improvements, bugfixing 
 * @author Franco Comida <fcomida@users.sourceforge.net>
 *
 */

#include "ashikhmin02Thread.h"
#include "../Common/config.h"
#include "../Fileformat/pfsoutldrimage.h"

void pfstmo_ashikhmin02 (pfs::Frame*,bool,float,int, ProgressHelper *);

Ashikhmin02Thread::Ashikhmin02Thread(pfs::Frame *frame, const TonemappingOptions &opts) : 
	TMOThread(frame, opts) {
}

void Ashikhmin02Thread::run() {
	connect(ph, SIGNAL(valueChanged(int)), this, SIGNAL(setValue(int)));
	emit setMaximumSteps(100);
	try {
		pfstmo_ashikhmin02(workingframe,
		opts.operator_options.ashikhminoptions.simple,
		opts.operator_options.ashikhminoptions.lct,
		opts.operator_options.ashikhminoptions.eq2 ? 2 : 4, ph);
	}
	catch(pfs::Exception e) {
		emit tmo_error(e.getMessage());
		emit deleteMe(this);
		return;
	}
	catch(...) {
		emit tmo_error("Failed to tonemap image");
		emit deleteMe(this);
		return;
	}
	
	if (!(ph->isTerminationRequested())) {
		const QImage& res = fromLDRPFStoQImage(workingframe);
		emit imageComputed(res);
	}
	emit finished();
	emit deleteMe(this);
}
//
// run()
//
