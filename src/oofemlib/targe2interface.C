/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2013   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "targe2interface.h"
#include "errorestimator.h"
#include "domain.h"
#include "node.h"
#include "element.h"
#include "remeshingcrit.h"
#include "classfactory.h"

namespace oofem {
REGISTER_Mesher(Targe2Interface, MPT_TARGE2);

MesherInterface :: returnCode
Targe2Interface :: createMesh(TimeStep *tStep, int domainNumber, int domainSerNum, Domain **dNew)
{
    * dNew = NULL;
    if ( this->createInput(this->domain, tStep) ) {
        return MI_NEEDS_EXTERNAL_ACTION;
    } else {
        return MI_FAILED;
    }
}

int
Targe2Interface :: createInput(Domain *d, TimeStep *tStep)
{
    FILE *outputStrem;
    RemeshingCriteria *rc = d->giveErrorEstimator()->giveRemeshingCrit();

    outputStrem = fopen("targe2.bmf", "w");
    // print header for 2D

    for ( auto &ielem : domain->giveElements() ) {
        fprintf( outputStrem, "MC-T %e %e %e %e %e %e   %e %e %e\n",
                ielem->giveNode(1)->giveCoordinate(1), ielem->giveNode(1)->giveCoordinate(2),
                ielem->giveNode(2)->giveCoordinate(1), ielem->giveNode(2)->giveCoordinate(2),
                ielem->giveNode(3)->giveCoordinate(1), ielem->giveNode(3)->giveCoordinate(2),
                rc->giveRequiredDofManDensity(ielem->giveNode(1)->giveNumber(), tStep),
                rc->giveRequiredDofManDensity(ielem->giveNode(2)->giveNumber(), tStep),
                rc->giveRequiredDofManDensity(ielem->giveNode(3)->giveNumber(), tStep) );
    }

    fclose(outputStrem);

    OOFEM_LOG_INFO("Targe2 .bmf file created\n");
    return 1;
}
} // end namespace oofem
