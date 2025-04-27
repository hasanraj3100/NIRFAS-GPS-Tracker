class _LocationTrackerState extends State<LocationTracker> with SingleTickerProviderStateMixin {
  double latitude = 0.0;
  double longitude = 0.0;
  double zoom = 13.0;
  final String url = 'YOUR_API_URL_HERE';
  late Timer _timer;
  final MapController _mapController = MapController();
  double currentLatitude = 0.0;
  double currentLongitude = 0.0;
  bool userHasMovedMap = false; // Track whether user has moved the map

  @override
  void initState() {
    super.initState();
    _fetchLocationData();
    _timer = Timer.periodic(Duration(seconds: 5), (_) => _fetchLocationData());
  }

  @override
  void dispose() {
    _timer.cancel();
    super.dispose();
  }

  // Fetch data from the API
  Future<void> _fetchLocationData() async {
    try {
      final response = await http.get(Uri.parse(url));
      if (response.statusCode == 200) {
        final data = json.decode(response.body);
        setState(() {
          latitude = data['latitude'];
          longitude = data['longitude'];
        });

        // Update the marker without moving the map if the user has moved the map
        if (!userHasMovedMap) {
          _mapController.move(LatLng(latitude, longitude), zoom);
        }
      } else {
        throw Exception('Failed to load location');
      }
    } catch (e) {
      print('Error fetching data: $e');
    }
  }

  // Track map movements to detect if the user manually moved the map
  void _onPositionChanged(MapPosition position, bool hasGesture) {
    if (hasGesture) {
      setState(() {
        userHasMovedMap = true;
      });
    } else {
      setState(() {
        userHasMovedMap = false;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Device Location Tracker'),
      ),
      body: FlutterMap(
        mapController: _mapController,
        options: MapOptions(
          center: LatLng(latitude, longitude),
          zoom: zoom,
          onPositionChanged: (position, hasGesture) {
            _onPositionChanged(position, hasGesture); // Track map position changes
          },
        ),
        children: [
          TileLayer(
            urlTemplate: 'https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png',
            subdomains: ['a', 'b', 'c'],
          ),
          MarkerLayer(
            markers: [
              Marker(
                point: LatLng(latitude, longitude),
                width: 40.0,
                height: 40.0,
                child: Container(
                  width: 40.0,
                  height: 40.0,
                  decoration: BoxDecoration(
                    color: Colors.red,
                    shape: BoxShape.circle,
                  ),
                ),
              ),
            ],
          ),
        ],
      ),
    );
  }
}