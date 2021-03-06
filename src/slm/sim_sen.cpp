/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
 *
 * Copyright (c) 2007 Sun Microsystems, Inc. All Rights Reserved.
 *
 * The contents of this file are subject to the terms of either the GNU Lesser
 * General Public License Version 2.1 only ("LGPL") or the Common Development and
 * Distribution License ("CDDL")(collectively, the "License"). You may not use this
 * file except in compliance with the License. You can obtain a copy of the CDDL at
 * http://www.opensource.org/licenses/cddl1.php and a copy of the LGPLv2.1 at
 * http://www.opensource.org/licenses/lgpl-license.php. See the License for the
 * specific language governing permissions and limitations under the License. When
 * distributing the software, include this License Header Notice in each file and
 * include the full text of the License in the License file as well as the
 * following notice:
 *
 * NOTICE PURSUANT TO SECTION 9 OF THE COMMON DEVELOPMENT AND DISTRIBUTION LICENSE
 * (CDDL)
 * For Covered Software in this distribution, this License shall be governed by the
 * laws of the State of California (excluding conflict-of-law provisions).
 * Any litigation relating to this License shall be subject to the jurisdiction of
 * the Federal Courts of the Northern District of California and the state courts
 * of the State of California, with venue lying in Santa Clara County, California.
 *
 * Contributor(s):
 *
 * If you wish your version of this file to be governed by only the CDDL or only
 * the LGPL Version 2.1, indicate your decision by adding "[Contributor]" elects to
 * include this software in this distribution under the [CDDL or LGPL Version 2.1]
 * license." If you don't indicate a single choice of license, a recipient has the
 * option to distribute your version of this file under either the CDDL or the LGPL
 * Version 2.1, or to extend the choice of license to its licensees as provided
 * above. However, if you add LGPL Version 2.1 code and therefore, elected the LGPL
 * Version 2 license, then the option applies only if the new code is made subject
 * to such option by the copyright holder.
 */

#include <stdlib.h>
#include "sim_sen.h"

TWCHAR &
SIMCharReaderIterator::operator*() const
{
    FILE *fp = reader->fp;
    std::deque<TWCHAR> & buf = reader->buf;

    for (int i = buf.size(); i <= idx; ) {
        wint_t wch = fgetwc(fp);
        if (wch == 0) {
            wch = 0x20; // make it like a space
        }
        if (wch == WEOF) {
            if (feof(fp)) {
                buf.push_back(TWCHAR(0));
                return buf.back();
            } else {
                fprintf(stderr, "File read error %d\n", ferror(fp));
                throw new int(60);
            }
        } else {
            buf.push_back(wch);
            ++i;
        }
    }
    if (idx >= (int) buf.size() && buf.back() == WCH_NULL)
        return buf.back();
    return buf[idx];
}

SIMCharReaderIterator&
SIMCharReaderIterator::operator++()
{
    std::deque<TWCHAR>& buf = reader->buf;
    TWCHAR wch = this->operator*();
    if (wch) {
        if (idx == 0)
            buf.pop_front();
        else
            ++idx;
    }
    return *this;
}

SIMCharReaderIterator
SIMCharReaderIterator::operator+(int i)
{
    return SIMCharReaderIterator(reader, idx + i);
}

// -*- indent-tabs-mode: nil -*- vim:et:ts=4
