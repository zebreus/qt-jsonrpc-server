# JSON-RPC 2.0 qt-jsonrpc Erweiterung
## Übersicht
Das Ziel dieser Erweiterung ist es die Qt Mechanismen von Signalen und Slots in json-rpc 2.0 abzubilden. Dazu wird spezifiziert, wie Slots auf dem Server aufgerufen werden, wie Signale an den Server transportiert werden und wie der Client die hier beschriebenen Funktionen aktivieren kann. Ausserdem wird ein Mechanismus spezifiziert, der genutzt werden kann um die Schnittstelle des servers herauszufinden. Ausserdem werden Datentypen spetifiziert und wie sie in Json kodiert werden.

## Definitionen
Das meiste was in der [JSON-RPC 2.0 Spezifikation](https://www.jsonrpc.org/specification) gilt, gilt hier auch. 

## Kompatibilität
Solange die Erweiterung nicht vom Client explizit aktiviert wurde, ist die Verbindung normales JSON-RPC 2.0.

## Slots
Einen Slot aufzurufen funktioniert genauso, wie ein normaler Mehtodenaufruf. Um Slots auf dem Server aufzurufen, sendet der Client ein Request Objekt. Der einzige Unterschied zu normalen Request Objekten ist, dass die Parameter by-position übergeben werden müssen und nicht by-name.

## Signale
Nachdem das Senden von Signalen aktiviert vom Client aktiviert wurde, kann der Server Signale an den Client senden. Das senden von Signalen ist im Grunden ein Methodenaufruf von Server an den Client. Es wird erwartet, dass der Client auf diese Nachrichten nicht antwortet. Die Request Objekte die der Server an den Client schickt haben keine id, sind Notifications und die Parameter werden immer by-position übergeben.

## Besondere Methoden
### bool rpc.qt.activate()
Nachdem diese Methode vom Client aufgerufen wurde, kann der Server auf der aktuellen Verbindung Signale aussenden. Der Rückgabewert ist immer true.
### bool rpc.qt.deactivate()
Mit dieser Methode kann der Client den Server auffordern keine Signale mehr zu senden. Der Rückgabewert ist immer true.
### object rpc.qt.describe()
Mit dieser methode kann die Schnittstelle des Servers angefragt werden. Der R
#### Schnittstellen object
##### name
Name des Servers. Als Text. Optional
##### version
Version des Servers. Als Nummer. Optional.
##### slots
Array aus Methoden Signatuern
Jede Signatur ist ein Object mit den Feldern

* name: Name der Methode als Text
* return: Typ des Rückgabewerts der Methode. Formattiert als ??? Maybe text
* parameters: Array mit den Typen der Parameter. Formattiert als ??? Maybe text
##### signals
Genauso aufgebaut wie slots.

## Datentypen
Qt definiert über das meta-object System Datentypen die für Signale und Slots genutzt werden können. Hier wird angegeben, wie die Verschiedenen Datentypen heißen und wie sie in Json dargestellt werden können.


