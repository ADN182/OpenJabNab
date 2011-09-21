#include "plugin_stats.h"
#include "bunny.h"
#include "bunnymanager.h"
#include "log.h"

Q_EXPORT_PLUGIN2(plugin_stats, PluginStats)

PluginStats::PluginStats():PluginInterface("stats", "stats plugin", SystemPlugin)
{
}

PluginStats::~PluginStats() {}

void PluginStats::InitApiCalls()
{
	DECLARE_PLUGIN_API_CALL("getcolors()", PluginStats, Api_GetColors);
	DECLARE_PLUGIN_API_CALL("getplugins()", PluginStats, Api_GetPlugins);
	DECLARE_PLUGIN_API_CALL("getbunniesip()", PluginStats, Api_GetBunniesIP);
	DECLARE_PLUGIN_API_CALL("getbunniesstatus()", PluginStats, Api_GetBunniesStatus);
}

PLUGIN_API_CALL(PluginStats::Api_GetPlugins)
{
	Q_UNUSED(account);
	Q_UNUSED(hRequest);

	QList<QByteArray> listB =  BunnyManager::GetConnectedBunniesList();

	QMap<QString, QVariant> list;
	foreach(QByteArray id, listB)
	{
		Bunny * b = BunnyManager::GetBunny(id);
		QList<QString> plugins = b->GetListOfPlugins();
		foreach(QString plugin, plugins)
			list.insert(plugin, list.value(plugin).toInt() + 1);
	}

	return new ApiManager::ApiMappedList(list);
}

PLUGIN_API_CALL(PluginStats::Api_GetColors)
{
	Q_UNUSED(account);
	Q_UNUSED(hRequest);

	QList<QByteArray> listB =  BunnyManager::GetConnectedBunniesList();

	QMap<QString, QVariant> list;
	foreach(QByteArray id, listB)
	{
		Bunny * b = BunnyManager::GetBunny(id);
		QString color = b->GetPluginSetting("colorbreathing", "color", QString("violet")).toString();
		list.insert(color, list.value(color).toInt() + 1);
	}

	return new ApiManager::ApiMappedList(list);
}

PLUGIN_API_CALL(PluginStats::Api_GetBunniesIP)
{
	Q_UNUSED(account);
	Q_UNUSED(hRequest);

	QList<QByteArray> listB =  BunnyManager::GetConnectedBunniesList();

	QMap<QString, QVariant> list;
	foreach(QByteArray id, listB)
	{
		Bunny * b = BunnyManager::GetBunny(id);
		list.insert(QString(b->GetID()), b->GetGlobalSetting("LastIP"));
	}

	return new ApiManager::ApiMappedList(list);
}

PLUGIN_API_CALL(PluginStats::Api_GetBunniesStatus)
{
	Q_UNUSED(account);
	Q_UNUSED(hRequest);

	QList<QByteArray> listB =  BunnyManager::GetConnectedBunniesList();

	QMap<QString, QVariant> list;
	list.insert("awake", 0);
	list.insert("sleep", 0);
	foreach(QByteArray id, listB)
	{
		Bunny * b = BunnyManager::GetBunny(id);
		QString awake = b->IsSleeping() ? "sleep" : "awake";
		list.insert(awake, list.value(awake).toInt() + 1);
	}

	return new ApiManager::ApiMappedList(list);
}

