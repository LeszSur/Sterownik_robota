#ifndef LOCATIONHANDLER_H
#define LOCATIONHANDLER_H

#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

class LocationHandler : public QObject {
    Q_OBJECT
public:
    explicit LocationHandler(QObject *parent = nullptr);
    void positionUpdated(const QGeoPositionInfo &info);

    //QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(0);

    QGeoCoordinate getCurrentLocation();

private:
    QGeoCoordinate currentLocation;
    QGeoPositionInfoSource *source;

signals:

    //void positionUpdated(const QGeoPositionInfo &info);
    void coordinatesUpdated(double lat, double lng); };

#endif // LOCATIONHANDLER_H
