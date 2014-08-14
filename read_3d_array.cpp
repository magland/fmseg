#include "read_3d_array.h"
#include <QFileInfo>
#include "jniftiimage2.h"

Array3D read_3d_array(const QString &path) {
	Array3D X;
	if ((QFileInfo(path).suffix()=="nii")||(QFileInfo(path).suffix()=="hdr")||(QFileInfo(path).suffix()=="img")) {
		JNiftiImage2 NI;
		NI.read(path);
		X=NI.dataXYZ(0);
	}
	else if (QFileInfo(path).suffix()=="mda") {
		X.read(path);
	}
	else {
		qWarning() << "Unknown file type";
	}
	return X;
}