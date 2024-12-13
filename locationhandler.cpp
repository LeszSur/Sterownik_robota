#include "locationhandler.h"

LocationHandler::LocationHandler(QObject *parent) : QObject(parent) {}

void LocationHandler::positionUpdated(const QGeoPositionInfo &info) {

    if(info.isValid()) {
        QGeoCoordinate currentLocation = info.coordinate();
        double Poslat = currentLocation.latitude();
        double Poslng = currentLocation.longitude();
    emit coordinatesUpdated(Poslat, Poslng);
    }
}
