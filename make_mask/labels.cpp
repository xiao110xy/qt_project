﻿#include "labels.h"
#include "utils.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QColormap>
#include <QDebug>

LabelInfo::LabelInfo() {
	this->name = "unlabeled";
	this->categorie = "void";
	this->id = 0;
	this->id_categorie = 0;
	this->color = QColor(0, 0, 0);
	item = NULL;
}

LabelInfo::LabelInfo(QString name, QString categorie, int id, int id_categorie, QColor color) {
	this->name = name;
	this->categorie = categorie;
	this->id = id;
	this->id_categorie = id_categorie;
	this->color = color;
	item = NULL;
}

void LabelInfo::read(const QJsonObject &json) {
	id = json["id"].toInt();
	name = json["name"].toString();
	categorie = json["categorie"].toString();
	id_categorie = json["id_categorie"].toInt();
	QJsonArray jarray = json["color"].toArray();
	int r = jarray[0].toInt();
	int g = jarray[1].toInt();
	int b = jarray[2].toInt();
	color = QColor(r, g, b);
}

void LabelInfo::write(QJsonObject &json) const {
	json["id"] = id;
	json["name"] = name;
	json["categorie"] = categorie;
	json["id_categorie"] = id_categorie;
	QJsonArray jarray;
	jarray.append(color.red());
	jarray.append(color.green());
	jarray.append(color.blue());
	json["color"] = jarray;
}


void Name2Labels::read(const QJsonObject &json) {
	QJsonObject array = json["labels"].toObject();
	for (QJsonObject::iterator it = array.begin(); it != array.end(); it++) {
		QJsonObject object = it->toObject();
		LabelInfo label;
		label.read(object);
		(*this)[label.name] = label;
	}
}
void Name2Labels::write(QJsonObject &json) const {
	QMapIterator<QString, LabelInfo> it(*this);
	QJsonObject object;
	while (it.hasNext()) {
		it.next();
		const LabelInfo & label = it.value();
		QJsonObject child_object;
		label.write(child_object);
		object[it.key()] = child_object;
	}
	json["labels"] = object;
}


Id2Labels getId2Label(const Name2Labels& labels) {
	Id2Labels id_labels;
	QMapIterator<QString, LabelInfo> i(labels);
	while (i.hasNext()) {
		i.next();
		id_labels[i.value().id] = &i.value();
	}
	return id_labels;
}

Name2Labels defaulfLabels() {
	Name2Labels labels;
	// inspired by https://raw.githubusercontent.com/mcordts/cityscapesScripts/master/cityscapesscripts/helpers/labels.py
	labels["no"           ] = LabelInfo("no"           , "void"         , 0, 50, QColor(  0,  0,  0));
	labels["water"           ] = LabelInfo("water"           , "void"         , 50, 50, QColor(  0,  255,  0));
	labels["sky"         ] = LabelInfo("sky"         , "void"         , 255, 255, QColor(  255,  0,  0));
	//labels["license plate"       ] = LabelInfo("license plate"       , "vehicle"      ,  -1, 7, QColor(  0,  0,142));

	//QVector<QColor> cmap = colorMap(labels.size());
	//qDebug() << "cmlap size = " << cmap.size();
	//QMapIterator<QString, LabelInfo> it(labels);
	//int i = 0;
	//while (it.hasNext()) {
	//	it.next();
	//	labels[it.key()].color = cmap[i++];
	//}
	//labels["unlabeled"].color = QColor(0, 0, 0);
	
	return labels;

}